import secrets
from datetime import datetime, timezone

from flask import Blueprint, jsonify, request

from models import Device, db

devices_bp = Blueprint("devices", __name__)


@devices_bp.route("/api/devices", methods=["GET"])
def list_devices():
    """List all registered devices."""
    devices = Device.query.order_by(Device.created_at.desc()).all()
    return jsonify([d.to_dict() for d in devices])


@devices_bp.route("/api/devices/<int:device_id>", methods=["GET"])
def get_device(device_id):
    """Get a single device by ID."""
    device = Device.query.get_or_404(device_id)
    return jsonify(device.to_dict())


@devices_bp.route("/api/devices", methods=["POST"])
def register_device():
    """
    Register a new edge device.
    Body: { "name": "psoc6-01", "location": "Warehouse A" }
    Returns the device info including a generated API key.
    """
    data = request.get_json()

    if not data or not data.get("name") or not data.get("location"):
        return jsonify({"error": "name and location are required"}), 400

    # Check for duplicate name
    existing = Device.query.filter_by(name=data["name"]).first()
    if existing:
        return jsonify({"error": f"Device '{data['name']}' already exists"}), 409

    api_key = secrets.token_hex(32)

    device = Device(
        name=data["name"],
        location=data["location"],
        api_key=api_key,
        status="online",
        last_seen=datetime.now(timezone.utc),
    )

    db.session.add(device)
    db.session.commit()

    return jsonify(device.to_dict()), 201


@devices_bp.route("/api/devices/<int:device_id>", methods=["PUT"])
def update_device(device_id):
    """Update device name or location."""
    device = Device.query.get_or_404(device_id)
    data = request.get_json()

    if data.get("name"):
        device.name = data["name"]
    if data.get("location"):
        device.location = data["location"]

    db.session.commit()
    return jsonify(device.to_dict())


@devices_bp.route("/api/devices/<int:device_id>", methods=["DELETE"])
def delete_device(device_id):
    """Delete a device."""
    device = Device.query.get_or_404(device_id)
    db.session.delete(device)
    db.session.commit()
    return jsonify({"message": "Device deleted"}), 200
