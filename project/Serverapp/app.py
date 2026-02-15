import os

from flask import Flask
from flask_cors import CORS

from models import db


def create_app():
    app = Flask(__name__)

    # --- Configuration ---
    base_dir = os.path.abspath(os.path.dirname(__file__))
    app.config["SQLALCHEMY_DATABASE_URI"] = (
        f"sqlite:///{os.path.join(base_dir, 'inventory.db')}"
    )
    app.config["SQLALCHEMY_TRACK_MODIFICATIONS"] = False
    app.config["SECRET_KEY"] = os.environ.get("SECRET_KEY", "dev-secret-key")

    # --- CORS (allow Next.js frontend) ---
    CORS(app, resources={r"/api/*": {"origins": "*"}})

    # --- Database ---
    db.init_app(app)

    with app.app_context():
        db.create_all()

    # --- Register Blueprints ---
    from routes.detections import detections_bp
    from routes.devices import devices_bp
    from routes.inventory import inventory_bp

    app.register_blueprint(devices_bp)
    app.register_blueprint(detections_bp)
    app.register_blueprint(inventory_bp)

    # --- Health check ---
    @app.route("/api/health")
    def health():
        return {"status": "ok"}

    return app


if __name__ == "__main__":
    app = create_app()
    app.run(debug=True, host="0.0.0.0", port=5000)
