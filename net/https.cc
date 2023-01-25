#include "net/https.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

#include "curl/curl.h"

namespace net {
namespace https {

namespace {
void SetHeaders(CURL* curl, const std::map<std::string, std::string>& headers) {
  struct curl_slist* list = nullptr;
  for (const auto& [name, value] : headers) {
    list = curl_slist_append(list, (name + ": " + value).c_str());
  }
  curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
}

// Callback function to handle response data
size_t WriteToStringCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  std::string* response = static_cast<std::string*>(userdata);
  response->append(ptr, size * nmemb);
  return size * nmemb;
}

size_t WriteToFileCallback(char* ptr, size_t size, size_t nmemb, void* userdata) {
  std::ofstream* fileStream = static_cast<std::ofstream*>(userdata);
  fileStream->write(ptr, size * nmemb);
  return size * nmemb;
}

}  // namespace

std::string Get(const std::string& url, const std::map<std::string, std::string>& headers,
                WriteType write_type, const std::string& file_path) {
  CURL* curl = curl_easy_init();

  if (!curl) {
    throw std::runtime_error("Failed to initialize libcurl");
  }

  std::string response;

  std::ofstream file_stream;
  if (write_type == WriteType::TO_FILE) {
    file_stream.open(file_path);
    if (!file_stream) {
      throw std::runtime_error("Failed to open file for writing: " + file_path);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  if (write_type == WriteType::TO_STRING) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  } else {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file_stream);
  }

  SetHeaders(curl, headers);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    // throw std::runtime_error("Failed to make GET request: " +
    // std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);
  if (write_type == WriteType::TO_FILE) {
    file_stream.close();
  }

  return response;
}

std::string Post(const std::string& url, const std::map<std::string, std::string>& headers,
                 const std::string& data, WriteType write_type, const std::string& file_path) {
  CURL* curl = curl_easy_init();

  if (!curl) {
    throw std::runtime_error("Failed to initialize libcurl");
  }

  std::string response;

  std::ofstream file_stream;
  if (write_type == WriteType::TO_FILE) {
    file_stream.open(file_path);
    if (!file_stream) {
      throw std::runtime_error("Failed to open file for writing: " + file_path);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_POST, 1);
  curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.c_str());
  curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, data.size());

  if (write_type == WriteType::TO_STRING) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  } else {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file_stream);
  }

  SetHeaders(curl, headers);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    // throw std::runtime_error("Failed to make POST request: " +
    //                          std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);

  if (write_type == WriteType::TO_FILE) {
    file_stream.close();
  }

  return response;
}

std::string Delete(const std::string& url, const std::map<std::string, std::string>& headers,
                   WriteType write_type, const std::string& file_path) {
  CURL* curl = curl_easy_init();

  if (!curl) {
    throw std::runtime_error("Failed to initialize libcurl");
  }

  std::string response;

  std::ofstream file_stream;
  if (write_type == WriteType::TO_FILE) {
    file_stream.open(file_path);
    if (!file_stream) {
      throw std::runtime_error("Failed to open file for writing: " + file_path);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

  if (write_type == WriteType::TO_STRING) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  } else {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &file_stream);
  }

  SetHeaders(curl, headers);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
    // throw std::runtime_error("Failed to make DELETE request: " +
    //                          std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);

  if (write_type == WriteType::TO_FILE) {
    file_stream.close();
  }

  return response;
}

std::string MimePost(const std::string& url, const std::map<std::string, std::string>& headers,
                     const std::map<std::string, std::string>& form_data,
                     const std::map<std::string, std::string>& files, WriteType write_type,
                     const std::string& file_path) {
  CURL* curl = curl_easy_init();

  if (!curl) {
    throw std::runtime_error("Failed to initialize libcurl");
  }

  std::string response;
  std::ofstream fileStream;
  if (write_type == WriteType::TO_FILE) {
    fileStream.open(file_path);
    if (!fileStream) {
      throw std::runtime_error("Failed to open file for writing: " + file_path);
    }
  }

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

  if (write_type == WriteType::TO_STRING) {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToStringCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
  } else {
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToFileCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &fileStream);
  }

  SetHeaders(curl, headers);

  // create a multipart formpost
  curl_mime* form = curl_mime_init(curl);

  // add form fields
  for (const auto& [name, value] : form_data) {
    curl_mimepart* field = curl_mime_addpart(form);
    curl_mime_data(field, value.c_str(), CURL_ZERO_TERMINATED);
    curl_mime_name(field, name.c_str());
  }

  // add file fields
  for (const auto& [name, filepath] : files) {
    curl_mimepart* field = curl_mime_addpart(form);
    curl_mime_filedata(field, filepath.c_str());
    curl_mime_name(field, name.c_str());
  }

  curl_easy_setopt(curl, CURLOPT_MIMEPOST, form);

  CURLcode res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    throw std::runtime_error("Failed to make MIME POST request: " +
                             std::string(curl_easy_strerror(res)));
  }

  curl_easy_cleanup(curl);
  curl_mime_free(form);
  if (write_type == WriteType::TO_FILE) {
    fileStream.close();
  }
  return response;
}

}  // namespace https
}  // namespace net