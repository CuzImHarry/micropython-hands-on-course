@echo off
setlocal

echo ==========================================
echo      Starting Setup for Serverapp
echo ==========================================

REM --- 1. Python Backend Setup ---
echo.
echo [1/4] Checking Python Virtual Environment...
if not exist "venv" (
    echo       Creating venv...
    python -m venv venv
) else (
    echo       venv already exists.
)

echo.
echo [2/4] Installing Python Dependencies...
call venv\Scripts\activate
pip install -r requirements.txt
if %errorlevel% neq 0 (
    echo Error installing python dependencies!
    pause
    exit /b %errorlevel%
)

REM --- 2. Node Frontend Setup ---
echo.
echo [3/4] Installing Frontend Node Modules...
cd frontend
if not exist "node_modules" (
    echo       node_modules not found, running npm install...
    call npm install
) else (
    echo       node_modules found. Running npm install to update...
    call npm install
)
if %errorlevel% neq 0 (
    echo Error installing node modules!
    pause
    exit /b %errorlevel%
)
cd ..

REM --- 3. Start Servers ---
echo.
echo ==========================================
echo [4/4] Starting Servers...
echo ==========================================

REM Start Backend
start "Flask Backend" cmd /k "venv\Scripts\activate && python app.py"

REM Start Frontend
cd frontend
start "Next.js Frontend" cmd /k "npm run dev"

echo.
echo Done! Servers should be running in new windows.
echo You can close this window.
pause
