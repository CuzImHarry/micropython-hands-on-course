import type { Metadata } from "next";
import "./globals.css";

export const metadata: Metadata = {
  title: "Edge AI Inventory Dashboard",
  description:
    "Real-time warehouse inventory management powered by Edge AI classification",
};

export default function RootLayout({
  children,
}: Readonly<{
  children: React.ReactNode;
}>) {
  return (
    <html lang="en">
      <body>{children}</body>
    </html>
  );
}
