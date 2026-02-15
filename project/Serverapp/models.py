from datetime import datetime, timezone
from flask_sqlalchemy import SQLAlchemy

db = SQLAlchemy()


class Device(db.Model):
    __tablename__ = "devices"

    id = db.Column(db.Integer, primary_key=True)
    name = db.Column(db.String(100), nullable=False, unique=True)
    location = db.Column(db.String(200), nullable=False)
    api_key = db.Column(db.String(64), nullable=False, unique=True)
    status = db.Column(db.String(20), default="online")
    last_seen = db.Column(
        db.DateTime, default=lambda: datetime.now(timezone.utc)
    )
    created_at = db.Column(
        db.DateTime, default=lambda: datetime.now(timezone.utc)
    )

    inventory_items = db.relationship(
        "InventoryItem", backref="device", lazy=True, cascade="all, delete-orphan"
    )
    detection_events = db.relationship(
        "DetectionEvent", backref="device", lazy=True, cascade="all, delete-orphan"
    )

    def to_dict(self):
        return {
            "id": self.id,
            "name": self.name,
            "location": self.location,
            "api_key": self.api_key,
            "status": self.status,
            "last_seen": self.last_seen.isoformat() if self.last_seen else None,
            "created_at": self.created_at.isoformat() if self.created_at else None,
        }


class InventoryItem(db.Model):
    __tablename__ = "inventory_items"

    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(
        db.Integer, db.ForeignKey("devices.id"), nullable=False
    )
    label = db.Column(db.String(100), nullable=False)
    status = db.Column(db.String(20), default="detected")  # detected / cleared
    confidence = db.Column(db.Float, default=0.0)
    updated_at = db.Column(
        db.DateTime,
        default=lambda: datetime.now(timezone.utc),
        onupdate=lambda: datetime.now(timezone.utc),
    )

    def to_dict(self):
        return {
            "id": self.id,
            "device_id": self.device_id,
            "device_name": self.device.name if self.device else None,
            "device_location": self.device.location if self.device else None,
            "label": self.label,
            "status": self.status,
            "confidence": self.confidence,
            "updated_at": self.updated_at.isoformat() if self.updated_at else None,
        }


class DetectionEvent(db.Model):
    __tablename__ = "detection_events"

    id = db.Column(db.Integer, primary_key=True)
    device_id = db.Column(
        db.Integer, db.ForeignKey("devices.id"), nullable=False
    )
    label = db.Column(db.String(100), nullable=False)
    confidence = db.Column(db.Float, nullable=False)
    timestamp = db.Column(
        db.DateTime, default=lambda: datetime.now(timezone.utc)
    )

    def to_dict(self):
        return {
            "id": self.id,
            "device_id": self.device_id,
            "device_name": self.device.name if self.device else None,
            "label": self.label,
            "confidence": self.confidence,
            "timestamp": self.timestamp.isoformat() if self.timestamp else None,
        }
