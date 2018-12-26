/*
    __ _____ _____ _____
   |  |   __|     |   | |  JSON for C++
 __|  |__   |  |  | | | |  version 1.0
|_____|_____|_____|_|___|  author atthawut phuangsiri

THIS LIBRARY WRITTED IN C++ 11
CREATE FOR READ/WRITE JSON STRING FORMAT IN C++ LANGUAGE

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

you can use in OOP concept 
class JSONObject for json object {    }
class JSONArray for json array [   ]
send your json string to constructor of JSONObject or JSONArray class for read json data :)

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

Example

std::string data = R"(
	{
		"name" : "atthawut",
		"age" : 23,
		"hobbies" : [
			"coding" ,
			"watching movies",
			"play games"
		]
	}
)";

JSONObject object(data);
object.getString("name");  ===> atthawut
object.getInt("age");	   ===> 23

JSONArray hobbies = object.getJSONArray("hobbies");
hobbies.getString(0)	   ===> coding	
hobbies.getString(2)	   ===> play games

- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

good luck.
atthawut phuangsiri.

*/

#include<vector>
#include<string>
#include <sstream>

class JSONObject;

class JSONArray {
private:
	std::vector<std::string> json_data;
public:
	JSONArray(std::string data) {
		data = data.substr(1, data.length() - 2);
		std::istringstream ss(data);
		std::string token;
		while (std::getline(ss, token, ',')) {
			json_data.push_back(token);
		}
	}
	JSONArray() {}
	std::string getString(int index) {
		std::string temp = json_data[index];
		std::string result;
		for (int i = 0; i < temp.length(); i++) {
			if (temp[i] != '"')
				result += temp[i];
		}
		return result;
	}
	
	int getInt(int index) {
		return stoi(json_data[index]);
	}
	double getDouble(int index) {
		return stod(json_data[index]);
	}
	JSONObject getJSONObject(int index);
	std::string toString() {
		std::string data = "[";
		int i = 0;
		for (auto d : json_data) {
			data += d + (i != json_data.size() - 1 ? "," : "");
			i++;
		}
		data += "]";
		return data;
	}
	void put(std::string value) {
		json_data.push_back(value);
	}
	void put(int value) {
		json_data.push_back(std::to_string(value));
	}
	void put(double value) {
		json_data.push_back(std::to_string(value));
	}
	void put(JSONObject json_object);

	void put(JSONArray json_array) {
		json_data.push_back(json_array.toString());
	}
	void put(int index, std::string value) {
		json_data[index] = value;
	}
	void put(int index, int value) {
		json_data[index] = std::to_string(value);
	}
	void put(int index, double value) {
		json_data[index] = std::to_string(value);
	}
	void put(int index, JSONObject json_object);
	void put(int index, JSONArray json_array) {
		json_data[index] = json_array.toString();
	}
	int size() {
		return json_data.size();
	}
};

class JSONObject {
private:
	std::vector<std::string> json_data;
	std::string fittData(std::string src) {
		std::vector<char> temp;
		int start = -1;
		int end = -1;
		for (int i = 0; i < src.length(); i++) {
			if (src[i] != 10 && src[i] != 9) {
				if (src[i] == '"') {
					if (start == -1) start = i;
					else end = i;
				}
				else if (src[i] != 32) {
					if (start == -1 && end == -1) {
						temp.push_back(src[i]);
					}
				}
				if (start != -1 || end != -1) {
					temp.push_back(src[i]);
				}
				if (start != -1 && end != -1) {
					start = end = -1;
				}
			}
		}
		char * res = new char[temp.size() + 1];
		for (int i = 0; i < temp.size(); i++) {
			res[i] = temp[i];
		}
		res[temp.size()] = '\0';
		return res;
	}
	std::vector<std::string> spliteObject(std::string data) {
		data = data.substr(1, data.length() - 2);
		std::vector<std::string> object;
		int old_index = 0;
		int start = -1;
		int end = -1;
		for (int i = 0; i < data.length(); i++) {
			if (data[i] == ',' ) {
				std::string d = data.substr(old_index, i - old_index);
				int c_index = d.find(":");
				bool isNum = (d[c_index] == ':' &&  d[c_index - 1] == '"' &&  d[c_index + 1] != '"');
				if (data[i - 1] == '"' || isNum) {
					old_index = i + 1;
					object.push_back(d);
				}
			}
			if (data[i] == '[') {
				std::string d = data.substr(i);
				i += d.find_last_of("]");
			}
			if (data[i] == '{') {
				std::string d = data.substr(i);
				i += d.find_last_of("}");
			}
			if (i == data.length() - 1) {
				std::string d = data.substr(old_index);
				object.push_back(d);
			}
		}
		return object;
	}
	std::string getKey(std::string token) {
		return token.substr(0, token.find(":"));
	}
	int indexOf(std::string key) {
		for (int i = 0; i < json_data.size(); i++) {
			if (getKey(json_data[i]) == key)
				return i;
		}
		return -1;
	}
	void m_put(std::string key, std::string value) {
		int index = this->indexOf(key);
		if (index != -1) json_data[index] = value;
		else json_data.push_back(value);
	}
public:
	JSONObject(std::string data) {
		std::string fitData = fittData(data);
		json_data = spliteObject(fitData);
	}
	JSONObject() {}
	std::string toString() {
		std::string data = "{";
		int i = 0;
		for (auto d : json_data) {
			data += d + (i != json_data.size() - 1 ? "," : "");
			i++;
		}
		data += "}";
		return data;
	}
	std::string getString(std::string key) {
		std::string m_key = "\"" + key + "\"";
		int uindex = -1, findex = -1;
		for (auto d : json_data) {
			if (getKey(d) == m_key) {
				uindex = d.find(key) + key.length() + 2;
				if (d[uindex] == '"') {
					findex = d.substr(++uindex).find("\"");
				}
				else if (d[uindex] == '{') {
					findex = d.substr(uindex).find_last_of("}") + 1;
				}
				else if (d[uindex] == '[') {
					std::string tmp = d.substr(uindex);
					findex = d.substr(uindex).find_last_of("]") + 1;
				}
				else {
					findex = d.substr(uindex).find(",");
					if (findex == -1) {
						findex = d.substr(uindex).find_last_of("}");
					}
				}
				return d.substr(uindex, findex);
			}
		}
		return "";
	}
	int getInt(std::string key) {
		return stoi(getString(key));
	}
	double getDouble(std::string key) {
		return stod(getString(key));
	}
	JSONObject getJSONObject(std::string key) {
		return JSONObject(getString(key));
	}
	int size() {
		return json_data.size();
	}
	JSONArray getJSONArray(std::string key) {
		return JSONArray(getString(key));
	}
	bool containKey(std::string key) {
		for (auto d : json_data) {
			if (getKey(d) == key)
				return true;
		}
		return false;
	}
	void put(std::string key, std::string value) {
		std::string result = key + ":\"" + value + "\"";
		this->m_put(key, result);
	}
	void put(std::string key, int value) {
		std::string result = key + ":" + std::to_string(value);
		this->m_put(key, result);
	}
	void put(std::string key, double value) {
		std::string result = key + ":" + std::to_string(value);
		this->m_put(key, result);
	}
	void put(std::string key, JSONArray json_array) {
		std::string result = key + ":" + json_array.toString();
		this->m_put(key, result);
	}
	void put(std::string key, JSONObject json_object) {
		std::string result = key + ":" + json_object.toString();
		this->m_put(key, result);
	}
};

JSONObject JSONArray::getJSONObject(int index) {
	return JSONObject(json_data[index]);
}
void JSONArray::put(JSONObject json_object) {
	json_data.push_back(json_object.toString());
}
void JSONArray::put(int index, JSONObject json_object) {
	json_data[index] = json_object.toString();
}