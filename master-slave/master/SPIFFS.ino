void readConfigFile(fs::FS &fs)
{
  File file = fs.open("/localConfig.txt", FILE_READ);
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
  localFileConfig = JSON.parse(content);
  file.close();
}

void writeConfigFile(fs::FS&fs)
{
  File file = fs.open("/localConfig.txt", FILE_WRITE);
  if (file.print(localFileConfig))
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
    else if (file.name() != "/localConfig.txt" && file.name() != "localConfig.txt")
    {
      String fileName = file.name();
      Serial.println(String(index + 1) + ". " + fileName + "\tSIZE: " + String(file.size()));
      //      Serial.print(fileName);
      //      Serial.print("\tSIZE: ");
      //      Serial.println(file.size());
      localFileConfig["content_length"] = index + 1;
      localFileConfig["contents"][index] = fileName;
      index++;
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

bool renameFile(fs::FS &fs, const char *oldPath)
{
  //  Serial.printf("Renaming file %s to %s\r\n", oldPath, newPath);
  //  if (fs.rename(oldPath, newPath))
  //  {
  //    Serial.println("- file renamed");
  //    return true;
  //  }
  //  else
  //  {
  //    Serial.println("- rename failed");
  //    return false;
  //  }

  while (!Serial.available()) {}
  String newPath = Serial.readStringUntil('\r\n');
  newPath.trim();
  newPath = '/' + newPath + ".txt";
  if (fs.exists(newPath.c_str()))
  {
    Serial.println("The file already exists. Do you want to replace it?");
    while (!Serial.available()) {}
    char choice = Serial.read();
    if (choice == 'y' || choice == 'Y')
    {
      fs.remove(oldPath);
      fs.rename(oldPath, newPath.c_str());
      return true;
    }
    else
      return false;
  }
  else if (newPath == "localConfig")
  {
    Serial.println("- invalid file name");
    return false;
  }
  else if (fs.rename(oldPath, newPath.c_str()))
  {
    Serial.println("- file renamed");
    return true;
  }
  else
  {
    Serial.println("- rename failed");
    return true;
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
