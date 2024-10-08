#ifndef NET_HTTPS_H
#define NET_HTTPS_H

#include <map>
#include <string>

namespace net {
namespace https {

enum class WriteType { TO_STRING, TO_FILE };

std::string Get(const std::string& url, const std::map<std::string, std::string>& headers,
                WriteType write_type = WriteType::TO_STRING, const std::string& file_path = "",
                const std::string& username = "", const std::string& password = "");

/**
 * @brief 
 * 
 * @param url 
 * @param headers 
 * @param data 
 * @param write_type 
 * @param file_path 
 * @param username 
 * @param password 
 * @return std::string 
 * @throws std::runtime_error if can't connect
 */
std::string Post(const std::string& url, const std::map<std::string, std::string>& headers,
                 const std::string& data, WriteType write_type = WriteType::TO_STRING,
                 const std::string& file_path = "", const std::string& username = "",
                 const std::string& password = "");

std::string Delete(const std::string& url, const std::map<std::string, std::string>& headers,
                   WriteType write_type = WriteType::TO_STRING, const std::string& file_path = "",
                   const std::string& username = "", const std::string& password = "");

std::string MimePost(const std::string& url, const std::map<std::string, std::string>& headers,
                     const std::map<std::string, std::string>& form_data,
                     const std::map<std::string, std::string>& files,
                     WriteType write_type = WriteType::TO_STRING, const std::string& file_path = "",
                     const std::string& username = "", const std::string& password = "");

}  // namespace https
}  // namespace net

#endif