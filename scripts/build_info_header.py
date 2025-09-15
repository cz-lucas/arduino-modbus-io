import subprocess
from datetime import datetime
import os

# Output header file path
header_path = os.path.join("include", "build_info.h")

# Get Git commit hash (short)
try:
    git_hash = subprocess.check_output(["git", "rev-parse", "--short", "HEAD"]).decode().strip()
except Exception:
    git_hash = "unknown"

# Get current timestamp
timestamp = datetime.now().strftime("%Y-%m-%d_%H:%M:%S")

# Write to header file
with open(header_path, "w") as f:
    f.write(f'#ifndef BUILD_INFO_H\n')
    f.write(f'#define BUILD_INFO_H\n\n')
    f.write(f'#define GIT_COMMIT_HASH "{git_hash}"\n')
    f.write(f'#define GIT_COMMIT_STRING "Commit: {git_hash}"\n')
    f.write(f'#define BUILD_TIMESTAMP "{timestamp}"\n\n')
    f.write(f'#endif // BUILD_INFO_H\n')

print(f"Generated {header_path} with commit hash and timestamp.")
