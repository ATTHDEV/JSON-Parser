#include"JSON.h"
#include<iostream>

void main() {

	std::string data = R"(
	{
		"information": {
			"firstName" : "atthawut",
			"lastName" : "phuangsiri"
		},
		"age" : 23,
		"hobbies" : [
			"coding" ,
			"watching movies",
			"play games"
		]
	}
	)";

	JSONObject json_object(data);

	std::cout << "JSON = " << json_object.toString() << "\n\n";

	JSONObject information = json_object.getJSONObject("information");

	std::cout << "information = " << information.toString() << "\n\n";

	std::cout << "firstName = " << information.getString("firstName") << "\n\n";

	std::cout << "age = " << json_object.getInt("age") << "\n\n";

	JSONArray hobbies = json_object.getJSONArray("hobbies");

	std::cout << "hobbies = " << hobbies.toString() << "\n\n";

	std::cout << "hobbies[0] = "<< hobbies.getString(0) << "\n\n";

	std::cout << "hobbies[2] = " << hobbies.getString(2) << "\n\n";

	std::cin.get();
}