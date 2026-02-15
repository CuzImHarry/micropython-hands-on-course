from datetime import datetime, timezone

from flask import Blueprint, jsonify, request

from models import DetectionEvent, Device, InventoryItem, db

detections_bp = Blueprint("detections", __name__)


@detections_bp.route("/api/detections", methods=["POST"])
def create_detection():
    """
    Main endpoint for edge devices.
    Body: { "device_id": 1, "label": "empty_box", "confidence": 0.95 }
    Optionally include "status": "cleared" to mark an item as removed.
    Authenticated via X-API-Key header.
    """
    # --- Authenticate via API key ---
    api_key = request.headers.get("X-API-Key")
    if not api_key:
        return jsonify({"error": "X-API-Key header required"}), 401

    data = request.get_json()
    if not data:
        return jsonify({"error": "JSON body required"}), 400

    device_id = data.get("device_id")
    label = data.get("label")
    confidence = data.get("confidence", 0.0)
    status = data.get("status", "detected")

    if not device_id or not label:
        return jsonify({"error": "device_id and label are required"}), 400

    # Verify device exists and API key matches
    device = Device.query.get(device_id)
    if not device:
        return jsonify({"error": "Device not found"}), 404
    if device.api_key != api_key:
        return jsonify({"error": "Invalid API key for this device"}), 403

    # Update device last_seen
    device.last_seen = datetime.now(timezone.utc)
    device.status = "online"

    # Create detection event
    event = DetectionEvent(
        device_id=device_id,
        label=label,
        confidence=confidence,
    )
    db.session.add(event)

    # Upsert inventory item (one per device + label combination)
    item = InventoryItem.query.filter_by(
        device_id=device_id, label=label
    ).first()

    if item:
        item.status = status
        item.confidence = confidence
        item.updated_at = datetime.now(timezone.utc)
    else:
        item = InventoryItem(
            device_id=device_id,
            label=label,
            status=status,
            confidence=confidence,
        )
        db.session.add(item)

    db.session.commit()

    return jsonify({
        "message": "Detection recorded",
        "event": event.to_dict(),
        "inventory_item": item.to_dict(),
    }), 201


@detections_bp.route("/api/detections", methods=["GET"])
def list_detections():
    """
    Query detection history.
    Query params: device_id, label, limit (default 50)
    """
    query = DetectionEvent.query

    device_id = request.args.get("device_id", type=int)
    if device_id:
        query = query.filter_by(device_id=device_id)

    label = request.args.get("label")
    if label:
        query = query.filter_by(label=label)

    limit = request.args.get("limit", 50, type=int)

    events = query.order_by(DetectionEvent.timestamp.desc()).limit(limit).all()
    return jsonify([e.to_dict() for e in events])
