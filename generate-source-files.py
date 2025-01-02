import os
def save_source_files(directory):
  """
  Saves paths to all .h and .cpp files within a directory to a file named "sources.list".

  Args:
    directory: The path to the directory to scan.
  """
  with open("sources.list", "w") as f:
    for root, dirs, files in os.walk(directory):
      for file in files:
        if file.endswith((".h", ".cpp")):
          filepath = os.path.join(root, file)
          f.write(filepath + "\n")

if __name__ == "__main__":
  import argparse

  parser = argparse.ArgumentParser(description="Save paths to .h and .cpp files in a directory.")
  parser.add_argument("directory", help="Path to the directory to scan.")
  args = parser.parse_args()

  save_source_files(args.directory)
  print("File paths saved to sources.list")
