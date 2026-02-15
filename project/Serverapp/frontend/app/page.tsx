"use client";

import { useEffect, useState, useCallback } from "react";
import {
  getDevices,
  getInventory,
  getInventoryStats,
  getDetections,
  Device,
  InventoryItem,
  DetectionEvent,
  InventoryStats,
} from "@/lib/api";
import StatsBar from "@/components/StatsBar";
import InventoryGrid from "@/components/InventoryGrid";
import EventFeed from "@/components/EventFeed";
import DeviceStatus from "@/components/DeviceStatus";

const REFRESH_INTERVAL = 5000; // 5 seconds

export default function DashboardPage() {
  const [stats, setStats] = useState<InventoryStats | null>(null);
  const [inventory, setInventory] = useState<InventoryItem[]>([]);
  const [events, setEvents] = useState<DetectionEvent[]>([]);
  const [devices, setDevices] = useState<Device[]>([]);
  const [loading, setLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [lastRefresh, setLastRefresh] = useState<Date>(new Date());

  const fetchData = useCallback(async () => {
    try {
      const [statsData, inventoryData, eventsData, devicesData] =
        await Promise.all([
          getInventoryStats(),
          getInventory(),
          getDetections(30),
          getDevices(),
        ]);

      setStats(statsData);
      setInventory(inventoryData);
      setEvents(eventsData);
      setDevices(devicesData);
      setError(null);
      setLastRefresh(new Date());
    } catch (err) {
      setError(
        err instanceof Error ? err.message : "Failed to connect to server"
      );
    } finally {
      setLoading(false);
    }
  }, []);

  useEffect(() => {
    fetchData();
    const interval = setInterval(fetchData, REFRESH_INTERVAL);
    return () => clearInterval(interval);
  }, [fetchData]);

  return (
    <div className="dashboard">
      {/* Header */}
      <header className="dashboard-header">
        <div>
          <h1>Edge AI Inventory</h1>
          <p className="subtitle">Real-time warehouse monitoring dashboard</p>
        </div>
        <div className="header-status">
          {error ? (
            <>
              <div className="status-dot" style={{ background: "var(--accent-rose)" }} />
              <span>Disconnected</span>
            </>
          ) : (
            <>
              <div className="status-dot" />
              <span>
                Live · Updated{" "}
                {lastRefresh.toLocaleTimeString([], {
                  hour: "2-digit",
                  minute: "2-digit",
                  second: "2-digit",
                })}
              </span>
            </>
          )}
        </div>
      </header>

      {/* Error Banner */}
      {error && (
        <div
          className="glass-card"
          style={{
            padding: "14px 20px",
            marginBottom: 24,
            borderColor: "var(--accent-rose)",
            color: "var(--accent-rose)",
            fontSize: "0.85rem",
          }}
        >
          ⚠️ {error} — Make sure the Flask backend is running on port 5000.
        </div>
      )}

      {/* Stats */}
      <StatsBar stats={stats} loading={loading} />

      {/* Main Grid */}
      <div className="main-grid">
        {/* Left — Inventory */}
        <InventoryGrid items={inventory} loading={loading} />

        {/* Right — Sidebar */}
        <div className="sidebar">
          <EventFeed events={events} loading={loading} />
          <DeviceStatus devices={devices} loading={loading} />
        </div>
      </div>
    </div>
  );
}
