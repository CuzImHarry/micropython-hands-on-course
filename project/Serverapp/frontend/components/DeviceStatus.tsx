"use client";

import { Device } from "@/lib/api";

interface DeviceStatusProps {
    devices: Device[];
    loading: boolean;
}

function formatLastSeen(iso: string | null): string {
    if (!iso) return "Never";
    const d = new Date(iso);
    const now = new Date();
    const diff = Math.floor((now.getTime() - d.getTime()) / 1000);

    if (diff < 60) return "Just now";
    if (diff < 3600) return `${Math.floor(diff / 60)}m ago`;
    if (diff < 86400) return `${Math.floor(diff / 3600)}h ago`;
    return d.toLocaleDateString();
}

function isOnline(lastSeen: string | null): boolean {
    if (!lastSeen) return false;
    const diff = Date.now() - new Date(lastSeen).getTime();
    return diff < 5 * 60 * 1000; // 5 minutes
}

export default function DeviceStatus({ devices, loading }: DeviceStatusProps) {
    return (
        <div className="glass-card devices-panel">
            <div className="section-header">
                <h2>Devices</h2>
                <span className="badge">{devices.length}</span>
            </div>
            {loading ? (
                <div style={{ display: "flex", flexDirection: "column", gap: 10 }}>
                    {[...Array(3)].map((_, i) => (
                        <div key={i} className="loading-shimmer" style={{ width: "100%", height: 52 }} />
                    ))}
                </div>
            ) : devices.length === 0 ? (
                <div className="empty-state">
                    <div className="empty-icon">📡</div>
                    <p>No devices registered yet.</p>
                </div>
            ) : (
                <div className="device-list">
                    {devices.map((device) => {
                        const online = isOnline(device.last_seen);
                        return (
                            <div key={device.id} className="device-item">
                                <div className={`device-status-indicator ${online ? "online" : "offline"}`} />
                                <div className="device-info">
                                    <div className="device-name">{device.name}</div>
                                    <div className="device-location">📍 {device.location}</div>
                                </div>
                                <div className="device-last-seen">{formatLastSeen(device.last_seen)}</div>
                            </div>
                        );
                    })}
                </div>
            )}
        </div>
    );
}
