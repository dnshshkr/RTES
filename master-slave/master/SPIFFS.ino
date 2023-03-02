void spiffsMain()
{
  Serial.println("***************FILE BROWSER***************");
  listDir(SPIFFS, "/", 0);
  Serial.println("\nMenu:\nO - Open File");
  Serial.println("R - Rename File");
  Serial.println("D - Delete File");
  Serial.println("U - Upload File");
  Serial.println("E - Exit");
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  Serial.printf("Storage list: %s\r\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("Failed to open directory");
    return;
  }
  if (!root.isDirectory())
  {
    Serial.println("Not a directory");
    return;
  }
  File file = root.openNextFile();
  while (file)
  {
    if (file.isDirectory())
    {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels)
        listDir(fs, file.path(), levels - 1);
    }
    else
    {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return;
  }
  Serial.println("- read from file:");
  while (file.available())
    Serial.write(file.read());
  file.close();
}

void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\r\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("- failed to open file for writing");
    return;
  }
  if (file.print(message))
    Serial.println("- file written");
  else
    Serial.println("- write failed");
  file.close();
}

void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\r\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("- failed to open file for appending");
    return;
  }
  if (file.print(message))
    Serial.println("- message appended");
  else
    Serial.println("- append failed");
  file.close();
}

void renameFile(fs::FS &fs, const char *oldPath, const char *newPath)
{
  Serial.printf("Renaming file %s to %s\r\n", oldPath, newPath);
  if (fs.rename(oldPath, newPath))
    Serial.println("- file renamed");
  else
    Serial.println("- rename failed");
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path))
    Serial.println(" - file deleted");
  else
    Serial.println(" - delete failed");
}
