import os

def create_android_mk(source_file_list_path, output_filename="Android.mk"):
  """
  Creates an Android.mk file listing source files from a provided file containing paths.

  Args:
    source_file_list_path: Path to the file containing the list of source file paths.
    output_filename: The name of the output Android.mk file (default: "Android.mk").
  """
  with open(output_filename, "w") as f:
    f.write("LOCAL_PATH := $(call my-dir)\n")
    f.write("include $(CLEAR_VARS)\n\n")
    f.write("LOCAL_MODULE := MyModuleName\n\n")  # Replace with your module name

    f.write("LOCAL_SRC_FILES := \\")

    with open(source_file_list_path, "r") as source_file_list:
      for line in source_file_list:
        source_file = line.strip()
        if source_file:
          # Extract filename and extension
          filename = os.path.basename(source_file)
          f.write(f"    $(wildcard $(LOCAL_PATH)/{filename}) \\")

    f.write("\n")
    f.write("include $(call all-subdir-makefiles)\n")

def main():
  """
  Reads source files from "sources.list" and creates an Android.mk file.
  """
  source_file_list_path = "sources.list"

  create_android_mk(source_file_list_path)
  print(f"Created Android.mk with source files from {source_file_list_path}")

if __name__ == "__main__":
  main()
