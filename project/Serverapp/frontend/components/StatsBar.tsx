"use client";

import { InventoryStats } from "@/lib/api";

interface StatsBarProps {
    stats: InventoryStats | null;
    loading: boolean;
}

export default function StatsBar({ stats, loading }: StatsBarProps) {
    if (loading || !stats) {
        return (
            <div className="stats-bar">
                {[...Array(4)].map((_, i) => (
                    <div key={i} className="glass-card stat-card">
                        <div className="loading-shimmer" style={{ width: 48, height: 48 }} />
                        <div className="stat-info">
                            <div className="loading-shimmer" style={{ width: 60, height: 24, marginBottom: 6 }} />
                            <div className="loading-shimmer" style={{ width: 80, height: 12 }} />
                        </div>
                    </div>
                ))}
            </div>
        );
    }

    const cards = [
        {
            icon: "📦",
            color: "blue",
            value: stats.total_items,
            label: "Inventory Items",
        },
        {
            icon: "📡",
            color: "cyan",
            value: `${stats.active_devices}/${stats.total_devices}`,
            label: "Active Devices",
        },
        {
            icon: "⚡",
            color: "emerald",
            value: stats.recent_detections,
            label: "Recent (1h)",
        },
        {
            icon: "📊",
            color: "amber",
            value: stats.total_detections,
            label: "Total Detections",
        },
    ];

    return (
        <div className="stats-bar">
            {cards.map((card) => (
                <div key={card.label} className="glass-card stat-card">
                    <div className={`stat-icon ${card.color}`}>{card.icon}</div>
                    <div className="stat-info">
                        <h3>{card.value}</h3>
                        <p>{card.label}</p>
                    </div>
                </div>
            ))}
        </div>
    );
}
