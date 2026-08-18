#!/usr/bin/env bash
set -euo pipefail

root_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
db_name="${DB_NAME:-pndbase}"
db_user="${DB_USER:-pndbase_app}"
db_host="${DB_HOST:-127.0.0.1}"
db_port="${DB_PORT:-5432}"
settings_file="${SETTINGS_FILE:-$root_dir/settings_user.ini}"
load_test_data="${LOAD_TEST_DATA:-1}"

if [[ ${EUID:-$(id -u)} -ne 0 ]]; then
    echo "Run this script through sudo: sudo $0" >&2
    exit 1
fi

if [[ ! "$db_name" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]] || [[ ! "$db_user" =~ ^[A-Za-z_][A-Za-z0-9_]*$ ]]; then
    echo "DB_NAME and DB_USER must be simple PostgreSQL identifiers." >&2
    exit 1
fi

app_password="$(openssl rand -hex 16)"

role_exists="$(runuser -u postgres -- psql -X -Atqc "SELECT 1 FROM pg_roles WHERE rolname = '$db_user'" postgres)"
if [[ "$role_exists" == "1" ]]; then
    runuser -u postgres -- psql -X -v ON_ERROR_STOP=1 postgres \
        -c "ALTER ROLE \"$db_user\" LOGIN PASSWORD '$app_password' NOSUPERUSER NOCREATEDB NOCREATEROLE;"
else
    runuser -u postgres -- psql -X -v ON_ERROR_STOP=1 postgres \
        -c "CREATE ROLE \"$db_user\" LOGIN PASSWORD '$app_password' NOSUPERUSER NOCREATEDB NOCREATEROLE;"
fi

db_exists="$(runuser -u postgres -- psql -X -Atqc "SELECT 1 FROM pg_database WHERE datname = '$db_name'" postgres)"
if [[ "$db_exists" != "1" ]]; then
    runuser -u postgres -- createdb -O postgres "$db_name"
fi

runuser -u postgres -- psql -X -v ON_ERROR_STOP=1 -d "$db_name" -f "$root_dir/database/deploy.sql"
if [[ "$load_test_data" == "1" ]]; then
    runuser -u postgres -- psql -X -v ON_ERROR_STOP=1 -d "$db_name" -f "$root_dir/database/seed_test_data.sql"
fi
runuser -u postgres -- psql -X -v ON_ERROR_STOP=1 -d "$db_name" <<SQL
GRANT CONNECT ON DATABASE "$db_name" TO "$db_user";
GRANT USAGE ON SCHEMA public, test, library, messager, reports_library, logs TO "$db_user";
GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public, test, library, messager, reports_library, logs TO "$db_user";
GRANT USAGE, SELECT, UPDATE ON ALL SEQUENCES IN SCHEMA public, test, library, messager, reports_library, logs TO "$db_user";
GRANT EXECUTE ON ALL FUNCTIONS IN SCHEMA public, test, library, messager, reports_library, logs TO "$db_user";
ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public, test, library, messager, reports_library, logs
    GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO "$db_user";
ALTER DEFAULT PRIVILEGES FOR ROLE postgres IN SCHEMA public, test, library, messager, reports_library, logs
    GRANT USAGE, SELECT, UPDATE ON SEQUENCES TO "$db_user";
ALTER DATABASE "$db_name" SET DateStyle TO 'ISO, DMY';
SQL

python3 - "$settings_file" "$db_host" "$db_port" "$db_name" "$db_user" "$app_password" <<'PY'
from pathlib import Path
import sys

path = Path(sys.argv[1])
values = {
    "ipdatabase": sys.argv[2],
    "portdatabase": sys.argv[3],
    "databasename": sys.argv[4],
    "username": sys.argv[5],
    "password": sys.argv[6],
    "type_sign": "true",
    "sys_user_type": "false",
}
lines = path.read_text().splitlines() if path.exists() else ["[General]"]
if not any(line.strip() == "[General]" for line in lines):
    lines.insert(0, "[General]")
seen = set()
out = []
for line in lines:
    key = line.split("=", 1)[0].strip() if "=" in line else None
    if key in values:
        out.append(f"{key}={values[key]}")
        seen.add(key)
    else:
        out.append(line)
for key, value in values.items():
    if key not in seen:
        out.append(f"{key}={value}")
path.write_text("\n".join(out) + "\n")
PY

owner="${SUDO_USER:-root}"
if id "$owner" >/dev/null 2>&1; then
    chown "$owner":"$(id -gn "$owner")" "$settings_file"
fi
chmod 600 "$settings_file"

PGPASSWORD="$app_password" psql -w -X -h "$db_host" -p "$db_port" -U "$db_user" -d "$db_name" \
    -Atqc "SELECT current_user, current_database(), count(*) FROM information_schema.tables WHERE table_schema IN ('test','library','messager','reports_library','logs');"

echo "Database deployment completed."
echo "Application database: $db_name"
echo "Application role: $db_user"
echo "Settings updated: $settings_file"
echo "Synthetic test data loaded: $load_test_data"
echo "Bootstrap application login: admin / admin (change it after the first login)."
