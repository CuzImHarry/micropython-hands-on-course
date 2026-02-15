const API_BASE = process.env.NEXT_PUBLIC_API_URL || "http://localhost:5000";

// ---- Types ----

export interface Device {
  id: number;
  name: string;
  location: string;
  api_key: string;
  status: string;
  last_seen: string | null;
  created_at: string | null;
}

export interface InventoryItem {
  id: number;
  device_id: number;
  device_name: string | null;
  device_location: string | null;
  label: string;
  status: "detected" | "cleared";
  confidence: number;
  updated_at: string | null;
}

export interface DetectionEvent {
  id: number;
  device_id: number;
  device_name: string | null;
  label: string;
  confidence: number;
  timestamp: string | null;
}

export interface InventoryStats {
  total_items: number;
  items_by_label: Record<string, { detected: number; cleared: number }>;
  active_devices: number;
  total_devices: number;
  total_detections: number;
  recent_detections: number;
}

// ---- Fetchers ----

async function fetchJSON<T>(path: string): Promise<T> {
  const res = await fetch(`${API_BASE}${path}`, { cache: "no-store" });
  if (!res.ok) {
    throw new Error(`API error: ${res.status} ${res.statusText}`);
  }
  return res.json();
}

export function getDevices(): Promise<Device[]> {
  return fetchJSON("/api/devices");
}

export function getInventory(): Promise<InventoryItem[]> {
  return fetchJSON("/api/inventory");
}

export function getInventoryStats(): Promise<InventoryStats> {
  return fetchJSON("/api/inventory/stats");
}

export function getDetections(limit = 30): Promise<DetectionEvent[]> {
  return fetchJSON(`/api/detections?limit=${limit}`);
}
