"use client";

import { InventoryItem } from "@/lib/api";

interface InventoryGridProps {
    items: InventoryItem[];
    loading: boolean;
}

function formatTime(iso: string | null): string {
    if (!iso) return "—";
    const d = new Date(iso);
    const now = new Date();
    const diff = Math.floor((now.getTime() - d.getTime()) / 1000);

    if (diff < 60) return `${diff}s ago`;
    if (diff < 3600) return `${Math.floor(diff / 60)}m ago`;
    if (diff < 86400) return `${Math.floor(diff / 3600)}h ago`;
    return d.toLocaleDateString();
}

function formatLabel(label: string): string {
    return label.replace(/_/g, " ");
}

export default function InventoryGrid({ items, loading }: InventoryGridProps) {
    if (loading) {
        return (
            <div>
                <div className="section-header">
                    <h2>Inventory</h2>
                </div>
                <div className="inventory-grid">
                    {[...Array(4)].map((_, i) => (
                        <div key={i} className="glass-card inventory-card">
                            <div className="loading-shimmer" style={{ width: "100%", height: 80 }} />
                        </div>
                    ))}
                </div>
            </div>
        );
    }

    return (
        <div>
            <div className="section-header">
                <h2>Inventory</h2>
                <span className="badge">{items.length} items</span>
            </div>
            {items.length === 0 ? (
                <div className="glass-card empty-state">
                    <div className="empty-icon">📦</div>
                    <p>No inventory items yet. Send a detection from your device to get started.</p>
                </div>
            ) : (
                <div className="inventory-grid">
                    {items.map((item) => (
                        <div key={item.id} className="glass-card inventory-card">
                            <div className="inventory-card-header">
                                <span className="inventory-label">{formatLabel(item.label)}</span>
                                <span className={`status-badge ${item.status}`}>{item.status}</span>
                            </div>
                            <div className="inventory-meta">
                                <span>📡 {item.device_name || `Device #${item.device_id}`}</span>
                                <span>📍 {item.device_location || "Unknown"}</span>
                                <span>🕒 {formatTime(item.updated_at)}</span>
                                <span>
                                    Confidence: {(item.confidence * 100).toFixed(1)}%
                                    <div className="confidence-bar-track">
                                        <div
                                            className="confidence-bar-fill"
                                            style={{ width: `${item.confidence * 100}%` }}
                                        />
                                    </div>
                                </span>
                            </div>
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
}
