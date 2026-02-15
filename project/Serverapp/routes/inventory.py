from flask import Blueprint, jsonify, request

from models import DetectionEvent, Device, InventoryItem, db

inventory_bp = Blueprint("inventory", __name__)


@inventory_bp.route("/api/inventory", methods=["GET"])
def get_inventory():
    """
    Get current inventory state.
    Query params: device_id, label, status
    """
    query = InventoryItem.query

    device_id = request.args.get("device_id", type=int)
    if device_id:
        query = query.filter_by(device_id=device_id)

    label = request.args.get("label")
    if label:
        query = query.filter_by(label=label)

    status = request.args.get("status")
    if status:
        query = query.filter_by(status=status)

    items = query.order_by(InventoryItem.updated_at.desc()).all()
    return jsonify([i.to_dict() for i in items])


@inventory_bp.route("/api/inventory/stats", methods=["GET"])
def get_inventory_stats():
    """
    Aggregate stats for the dashboard.
    Returns total items by label, active device count, recent detection count.
    """
    # Total inventory items by label
    items = InventoryItem.query.all()
    label_counts = {}
    for item in items:
        if item.label not in label_counts:
            label_counts[item.label] = {"detected": 0, "cleared": 0}
        label_counts[item.label][item.status] = (
            label_counts[item.label].get(item.status, 0) + 1
        )

    # Active devices (seen in last 5 minutes)
    from datetime import datetime, timedelta, timezone

    threshold = datetime.now(timezone.utc) - timedelta(minutes=5)
    active_devices = Device.query.filter(Device.last_seen >= threshold).count()
    total_devices = Device.query.count()

    # Total detection events
    total_detections = DetectionEvent.query.count()

    # Recent detections (last hour)
    hour_ago = datetime.now(timezone.utc) - timedelta(hours=1)
    recent_detections = DetectionEvent.query.filter(
        DetectionEvent.timestamp >= hour_ago
    ).count()

    return jsonify({
        "total_items": len(items),
        "items_by_label": label_counts,
        "active_devices": active_devices,
        "total_devices": total_devices,
        "total_detections": total_detections,
        "recent_detections": recent_detections,
    })
