void readConfigFile(fs::FS &fs)
{
  File file = fs.open("/config.txt", FILE_READ);
  if (!file || file.isDirectory())
  {
    Serial.println("- failed to open file for reading");
    return;
  }
  else
    Serial.println("Flash storage configured");
  String content;
  while (file.available())
    content += file.read();
  fileConfig = JSON.parse(content);
  file.close();
}

void writeConfigFile(fs::FS&fs)
{
  File file = fs.open("/config.txt", FILE_WRITE);
  if (file.print(fileConfig))
    Serial.println("config file write successful");
  else
    Serial.println("config file write failed");
}

void listDir(fs::FS &fs, const char *dirname, uint8_t levels)
{
  byte index = 0;
  Serial.printf("Storage list: %s\r\n", dirname);
  File root = fs.open(dirname);
  if (!root)
  {
    Serial.println("- failed to open directory");
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
        listDir(fs, file.name(), levels - 1);
    }
    else
    {
      String fileName = file.name();
      if (fileName != "/config.txt")
      {
        Serial.println(String(index + 1) + ". " + fileName + "\tSIZE: " + String(file.size()));
        //      Serial.print(fileName);
        //      Serial.print("\tSIZE: ");
        //      Serial.println(file.size());
        fileConfig["content_length"] = index + 1;
        fileConfig["contents"][index] = fileName;
        index++;
      }
    }
    file = root.openNextFile();
  }
}

void readFile(fs::FS &fs, const char *path)
{
  Serial.printf("Reading file: %s\r\n", path);
  File file = fs.open(path, FILE_READ);
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

bool renameFile(fs::FS &fs, const char *oldPath, const char *newPath)
{
  Serial.printf("Renaming file %s to %s\r\n", oldPath, newPath);
  if (fs.rename(oldPath, newPath))
  {
    Serial.println("- file renamed");
    return true;
  }
  else
  {
    Serial.println("- rename failed");
    return false;
  }
}

void deleteFile(fs::FS &fs, const char *path)
{
  Serial.printf("Deleting file: %s\r\n", path);
  if (fs.remove(path))
    Serial.println("- file deleted");
  else
    Serial.println("- delete failed");
}
