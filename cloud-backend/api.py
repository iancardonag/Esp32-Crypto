# Minimal backend opcional (Flask) - se puede desplegar en Render/Railway
from flask import Flask, request, jsonify
app = Flask(__name__)

@app.get("/health")
def health():
    return {"ok": True}

# Guardar comandos (ejemplo simple en memoria)
pending_cmds = []

@app.post("/cmd")
def cmd():
    body = request.get_json(force=True)
    cmd = body.get("command", "")
    if not cmd:
        return {"error": "missing command"}, 400
    pending_cmds.append(cmd)
    return {"queued": cmd}

@app.get("/cmd/next")
def cmd_next():
    if pending_cmds:
        return {"command": pending_cmds.pop(0)}
    return {"command": ""}

if __name__ == "__main__":
    app.run(host="0.0.0.0", port=8000)