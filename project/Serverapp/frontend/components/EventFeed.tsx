"use client";

import { DetectionEvent } from "@/lib/api";

interface EventFeedProps {
    events: DetectionEvent[];
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

export default function EventFeed({ events, loading }: EventFeedProps) {
    return (
        <div className="glass-card event-feed">
            <div className="section-header">
                <h2>Live Events</h2>
                <span className="badge">LIVE</span>
            </div>
            {loading ? (
                <div style={{ display: "flex", flexDirection: "column", gap: 8 }}>
                    {[...Array(5)].map((_, i) => (
                        <div key={i} className="loading-shimmer" style={{ width: "100%", height: 40 }} />
                    ))}
                </div>
            ) : events.length === 0 ? (
                <div className="empty-state">
                    <div className="empty-icon">📡</div>
                    <p>Waiting for detection events...</p>
                </div>
            ) : (
                <div className="event-list">
                    {events.map((event) => (
                        <div key={event.id} className="event-item">
                            <div className="event-dot detected" />
                            <div className="event-info">
                                <div className="event-label">{formatLabel(event.label)}</div>
                                <div className="event-device">
                                    {event.device_name || `Device #${event.device_id}`} ·{" "}
                                    {(event.confidence * 100).toFixed(0)}%
                                </div>
                            </div>
                            <div className="event-time">{formatTime(event.timestamp)}</div>
                        </div>
                    ))}
                </div>
            )}
        </div>
    );
}
