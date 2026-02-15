Write-Host "=========================================="
Write-Host "      Starting Setup for Serverapp"
Write-Host "=========================================="

# 1. Python Backend Setup
Write-Host "`n[1/4] Checking Python Virtual Environment..."
if (-not (Test-Path "venv")) {
    Write-Host "       Creating venv..."
    python -m venv venv
} else {
    Write-Host "       venv already exists."
}

Write-Host "`n[2/4] Installing Python Dependencies..."
& .\venv\Scripts\Activate.ps1
pip install -r requirements.txt
if ($LASTEXITCODE -ne 0) {
    Write-Error "Error installing python dependencies!"
    # Pause equivalent in PowerShell
    Read-Host "Press Enter to exit..."
    exit $LASTEXITCODE
}

# 2. Node Frontend Setup
Write-Host "`n[3/4] Installing Frontend Node Modules..."
Push-Location frontend
if (-not (Test-Path "node_modules")) {
    Write-Host "       node_modules not found, running npm install..."
    npm install
} else {
    Write-Host "       node_modules found. Updating..."
    npm install
}
if ($LASTEXITCODE -ne 0) {
    Write-Error "Error installing node modules!"
    # Pause equivalent
    Read-Host "Press Enter to exit..."
    exit $LASTEXITCODE
}
Pop-Location

# 3. Start Servers
Write-Host "`n=========================================="
Write-Host "[4/4] Starting Servers..."
Write-Host "=========================================="

# Start Backend
Start-Process -FilePath "powershell" -ArgumentList "-NoExit", "-Command", "& { . .\venv\Scripts\Activate.ps1; python app.py }"

# Start Frontend
Push-Location frontend
Start-Process -FilePath "powershell" -ArgumentList "-NoExit", "-Command", "npm run dev"
Pop-Location

Write-Host "`nDone! Servers should be running in new windows."
Write-Host "You can close this window."
Start-Sleep -Seconds 5
