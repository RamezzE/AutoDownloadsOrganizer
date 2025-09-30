import shutil
from pathlib import Path

# Extensions to ignore (incomplete/temporary downloads)
EXTENSIONS_TO_IGNORE = {
    ".crdownload", ".part", ".download", ".partial",
    ".ut", ".tmp", ".filepart", ".incomplete", ".!download"
}

# Get Downloads folder
DOWNLOADS = Path.home() / "Downloads"

def should_ignore(file_path: Path) -> bool:
    return file_path.suffix.lower() in EXTENSIONS_TO_IGNORE

def create_folders(folder: Path, extensions: set[str]):
    ext_map = {}
    for ext in extensions:
        folder_name = (ext[1:].upper() + "s") if ext else "OTHERS"
        dest_folder = folder / folder_name
        dest_folder.mkdir(exist_ok=True)
        ext_map[ext] = dest_folder
    return ext_map

def move_file(file_path: Path, dest_folder: Path):
    dest_path = dest_folder / file_path.name
    i = 0
    while dest_path.exists():
        i += 1
        stem, ext = file_path.stem, file_path.suffix
        dest_path = dest_folder / f"{stem} ({i}){ext}"
    shutil.move(str(file_path), str(dest_path))
    print(f"Moved {file_path.name} → {dest_path}")

def organize_once(folder: Path):
    files = [f for f in folder.iterdir() if f.is_file() and not should_ignore(f)]
    exts = {f.suffix for f in files if f.suffix}
    ext_map = create_folders(folder, exts)
    for f in files:
        dest_folder = ext_map.get(f.suffix)
        if dest_folder:
            move_file(f, dest_folder)

if __name__ == "__main__":
    print(f"Organizing files in {DOWNLOADS}")
    organize_once(DOWNLOADS)
    print("Done ✅")
