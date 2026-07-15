#include "cstdio"
#include "cstdlib"
#include "vector"
#include "utility"
#include "string"

#include "nlohmann/json.hpp"
#include "xasm2_cmd.hpp"

enum toktype : uint8_t {
	TYPE_KEYWORD, // const
	TYPE_COMMAND, // nop, load, loadc, store
	TYPE_INTEGER, // 0, 1, 2, 0x20
	TYPE_FLOAT, // 1.0, 2.0
	TYPE_ADDRESS, // @main:, label:
	TYPE_IDENTIFIER, // Defined by const or address
	TYPE_PROC, // @ symbol
	TYPE_ENDANAME, // address_name:
	TYPE_COMMA, // Comma
	TYPE_UNKNOWN, // Any extra
};

//		   identifier,       kind      source       line
typedef std::tuple<const std::string, uint8_t, const char*, uint64_t> token_t;

inline std::vector<token_t> g_Tokens;
inline uint8_t* g_Buffer = nullptr;
inline size_t g_Index = 0;

int OpenReadFile(const char* source, size_t* size_out, char** text_out) {
	FILE* fp;
	char* text = nullptr;
	size_t size = 0;

	if(fp = fopen(source, "r")) {
		fseek(fp, 0, SEEK_END);
		size = ftell(fp);
		rewind(fp);
		text = (char*)calloc(1, size + 64); // 64 extra bytes to make sure I'm reading data
		fread(text, size, 1, fp);

		*text_out = text;
		*size_out = size;
		fclose(fp);
	}
	else {
		printf("File \"%s\" not found\n", source);
		return -1;
	}
	return 0;
}

int Tokenize(char* text, const char* source_name, size_t size) {
	//printf("Enter Tokenize\n");
	size_t line = 1;
	for(size_t i = 0; i < size; ) {
		char c = text[i];
		if(c == 0) break; // If we reach an empty spot (EOF)

		if(c == ' ' || c == '\t' || c == 0xa || c == 0x0d) {
			if(c == '\n') {
				line++;
			}
			i++;
		} 
		else if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')) { // identifier or keyword
			std::string s = "";
			toktype t = TYPE_IDENTIFIER;
			auto IsIdentifierC = [](char c) { return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9'); };

			auto IsKeyword = [] (const std::string& s) {
				return s == "const";
			};

			while(IsIdentifierC(text[i])) {
				s.push_back(text[i]);
				i++;
			}
			if(IsKeyword(s)) {
				t = TYPE_KEYWORD;
			}
			else if(cmd2byte.find(s) != cmd2byte.end() || cmd2byteEx.find(s) != cmd2byteEx.end()) {
				//printf("byte: 0x%x ", cmd2byte.at(s));
				t = TYPE_COMMAND;
			}
			//printf("%s\n", s.c_str());
			g_Tokens.emplace_back(std::make_tuple(s, t, source_name, line));
		}
		else if (c >= '0' && c <= '9' || c == '-') { // Number
			std::string n = "";
			toktype t = TYPE_INTEGER;

			if(text[i] == '0' && (text[i + 1] == 'x' || text[i + 1] == 'X')) {
				i += 2;
				auto IsNumber = [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); };

				while (IsNumber(text[i])) {
					n.push_back(text[i]);
					i++;
				}
				//printf("%s : %d\n", n.c_str(), std::stoll(n, nullptr, 16));
				n = std::to_string(std::stoll(n, nullptr, 16));
			}
			else {
				bool floating = false;
				auto IsNumber = [](char c) { return (c >= '0' && c <= '9'); };
				if(text[i] == '-') {
					n.push_back('-');
					i++;
				}
				while (IsNumber(text[i]) || (false == floating && text[i] == '.')) {
					if(text[i] == '.') {
						floating = true;
						t = TYPE_FLOAT;
					}
					n.push_back(text[i]);
					i++;
				}
				/*
				if(floating)
					printf("%s : %f\n", n.c_str(), std::stod(n));
				else 
					printf("%s : %d\n", n.c_str(), std::stoll(n));*/
			}
			g_Tokens.emplace_back(std::make_tuple(n, t, source_name, line));
		}
		else if (c == '@' || c == ':' || c == ',') { // Extra symbols to delimit things
			//printf("%c\n", c);
			std::string s = "";
			toktype t = TYPE_UNKNOWN;
			switch(c) {
				case '@': s = "@"; t = TYPE_PROC; break;
				case ':': s = ":"; t = TYPE_ENDANAME; break;
				case ',': s = ","; t = TYPE_COMMA; break;
			}

			g_Tokens.emplace_back(std::make_tuple(s, t, source_name, line));
			i++;
		}
		else if (c == ';') { // Comment
			while(text[i] != '\n') {
				i++;
			}
		}
		else {
			printf("%s - %d: Extra character %c\n", source_name, line, c);
			return -1;
		}

	}
	return 0;
}

int64_t ToData() {
	// Use g_Tokens and turn them into bytes
	uint8_t* buffer = (uint8_t*)realloc(g_Buffer, g_Index + 4 * g_Tokens.size());
	g_Buffer = buffer;
	size_t index = g_Index;
	size_t tok_size = g_Tokens.size();
	bool on_error = false;
	for(size_t i = 0; i < tok_size; i++) {
		const token_t& t = g_Tokens[i];
		if(on_error) {
error:
			printf("Error happened at \"%s\" - %d index was %d, value: %s\n", std::get<2>(g_Tokens[i]), std::get<3>(g_Tokens[i]), i, std::get<0>(g_Tokens[i]).c_str());
			return -1;
		}
		switch(std::get<1>(t)) {
			case TYPE_KEYWORD: // const
				printf("Not implemented yet\n");
				goto error;
				break;
			case TYPE_COMMAND: // nop, load, loadc, store
				{
					std::string cmd_name = std::get<0>(t);
					uint8_t cmd = 0;
					if(cmd2byte.find(cmd_name) != cmd2byte.end()) {
						cmd = (uint8_t)cmd2byte.at(cmd_name);
					} else {
						cmd = (uint8_t)cmd2byteEx.at(cmd_name);
					}
					buffer[index] = cmd;
					//printf("COMMAND: %s\n", std::get<0>(t).c_str());
					index++;
					i++;
					const std::map<uint8_t, std::vector<ARG_TYPE>>* arg_map = nullptr;
					if(g_Args.find(cmd) != g_Args.end()) {
						arg_map = &g_Args;
					} else if(g_ExtraCmdArgs.find(cmd) != g_ExtraCmdArgs.end()) {
						arg_map = &g_ExtraCmdArgs;
					} else {
						i--;
						break;
					}
					if(arg_map->find(cmd) != arg_map->end()) {
						size_t len = arg_map->at(cmd).size() * 2 - 1; // Comma between
						const std::vector<ARG_TYPE>& args = arg_map->at(cmd);
						for(int a = 0; a < len; a++) {
							if(i < tok_size) { 
								const std::string str = std::get<0>(g_Tokens[i]);
								uint8_t type = std::get<1>(g_Tokens[i]);
								i++;
								if((a % 2) && type == TYPE_COMMA) {
									continue;
								} else if(a % 2 == 0) {
									auto IsRegister = [] (const std::string& s) { 
										return s == "r1" || s == "r2" || s == "r3" || s == "r4";
									};
									switch(args[a >> 1]) {
										case ARG_REGISTER:
											if(type == TYPE_INTEGER) {
												buffer[index] = (uint8_t)std::stoll(str);
												index++;
											} else if(type == TYPE_IDENTIFIER && IsRegister(str)) {
												if(str == "r1") buffer[index] = 0; else if(str == "r2") buffer[index] = 1; else if(str == "r3") buffer[index] = 2; else if(str == "r4") buffer[index] = 3;
												index++;
											} else {
												printf("ARG_REGISTER: Expected integer\n");
												goto error;
											}

											break;
										case ARG_NUMBER:
											if(type == TYPE_INTEGER) {
												*(int32_t*)(buffer + index) = (int32_t)std::stoll(str);
												index += sizeof(int32_t);
											} else if (type == TYPE_FLOAT) {
												*(float*)(buffer + index) = (float)std::stod(str);
												index += sizeof(float);
											} else {
												printf("ARG_NUMBER: Expected integer or float\n");
												goto error;
											}
											break;
										case ARG_INTEGER:
											if(type == TYPE_INTEGER) {
												*(int32_t*)(buffer + index) = (int32_t)std::stoll(str);
												index += sizeof(int32_t);
											} else if (type == TYPE_FLOAT) {
												*(int32_t*)(buffer + index) = (int32_t)std::stod(str);
												index += sizeof(int32_t);
											} else {
												printf("ARG_INTEGER: Expected integer or float\n");
												goto error;
											}
											break;
										case ARG_FLOAT:
											if(type == TYPE_INTEGER) {
												*(float*)(buffer + index) = (float)std::stoll(str);
												index += sizeof(float);
											} else if (type == TYPE_FLOAT) {
												*(float*)(buffer + index) = (float)std::stod(str);
												index += sizeof(float);
											} else {
												printf("ARG_FLOAT: Expected integer or float\n");
												goto error;
											}
											break;
										case ARG_ADDRESS:
											if(type == TYPE_IDENTIFIER) {
												g_SymbolTable[str].push_back((uint32_t)index);
												index += 4;
											} else {
												printf("ARG_ADDRESS: Expected identifier, got %s\n", str.c_str());
												goto error;
											}
											break;
									}

								} else {
									printf("Missing comma\n");
									goto error;
								}
							}
						}
					}
					i--;
				}
				break;
			case TYPE_IDENTIFIER: // Defined by const or address
				if(i + 1 < tok_size && 
					std::get<1>(g_Tokens[i + 1]) == TYPE_ENDANAME
					) {
					g_AddressTable[std::get<0>(g_Tokens[i])] = index + 12;
					i++;
				}
				else {
					printf("Bad syntax (Should be label:)\n");
					goto error;
				}
				break;
			case TYPE_PROC: // @ symbol
				{
					if(i + 2 < tok_size && 
						std::get<1>(g_Tokens[i + 1]) == TYPE_IDENTIFIER &&
						std::get<1>(g_Tokens[i + 2]) == TYPE_ENDANAME
						) {
						g_AddressTable[std::get<0>(g_Tokens[i + 1])] = index + 12;
						i += 2;							
					}
					else {
						printf("Bad syntax (Should be @proc:)\n");
						goto error;
					}
				}
				break;
			default:
			case TYPE_UNKNOWN: // Any extra
				printf("Token out of order\n");
				goto error;
				break;
		}
	}
	return index;
}

void FillAddresses() {
	for(const auto& [proc, proc_address] : g_AddressTable) {
		printf("Proc name: \"%s\" address: 0x%x\n", proc.c_str(), proc_address);
		printf("Proc \"%s\" has %d mentions\n", proc.c_str(), g_SymbolTable[proc].size());
		for(const uint32_t& offset : g_SymbolTable[proc]) {
			printf("offset: 0x%x\n", offset);
			*(uint32_t*)(g_Buffer + offset) = proc_address;
		}
	}
}

void WriteBinary(const char* name, size_t size) {
	if(g_AddressTable.find("main") == g_AddressTable.end()) {
		printf("Can't find main procedure, stop\n");
		return;
	}
	FILE* fp = fopen(name, "wb");
	const char* magic = "XASM0002";
	uint32_t main_proc = g_AddressTable["main"];
	fwrite(magic, 8, 1, fp);
	fwrite(&main_proc, sizeof(uint32_t), 1, fp);
	fwrite(g_Buffer, size, 1, fp);
	fclose(fp);
}

void LoadCommandArgsFromJson(const char* name) {
	nlohmann::json json;
	char* text = nullptr;
	size_t size = 0;
	uint16_t cmd_idx = 0x80;
	printf("Loading command args from json\n");
	if(0 == OpenReadFile(name, &size, &text)) {
		try {
			json = nlohmann::json::parse(text);
			printf("Parsing \"%s\"\n", name);
			const auto& cmd_args = json["cmd"];
			for(const auto& cmd : cmd_args) {
				if(cmd.find("value") != cmd.end()) {
					cmd_idx = std::stoll((const std::string&)cmd["value"], nullptr, 16);
				}
				if(0x100 <= cmd_idx) { 
					printf("Reached extra command limit, stopping\n");
					break;
				}
				const std::string& name = cmd["name"];
				cmd2byteEx[name] = cmd_idx;
				printf("COMMAND: \"%s\", value %d 0x%x\n", name.c_str(), cmd2byteEx[name], cmd2byteEx[name]);
				if(cmd.find("args") != cmd.end()) {
					for(const std::string& argt : cmd["args"]) {
						printf("ARG: %s\n", argt.c_str());
						ARG_TYPE argtype = ARG_UNKNOWN;
						if(argt == "register") argtype == ARG_REGISTER;
						else if(argt == "number") argtype == ARG_NUMBER;
						else if(argt == "integer") argtype == ARG_INTEGER;
						else if(argt == "float") argtype == ARG_FLOAT;
						else if(argt == "address") argtype == ARG_ADDRESS;
						else continue;
						g_ExtraCmdArgs[cmd_idx].push_back(argtype);
					}
				}
				cmd_idx++;
			}
		}
		catch(const std::exception& e) {
			printf("JSON exception, stop processing: %s\n", e.what());
			return;
		}
		
		g_ExtraCmdArgs.clear();
	}

}

int main(int argc, char** argv) {
	if(argc < 2) {
		printf("Usage: program output source [source ...]\n");
		return -1;
	}
	bool on_error = false;
	for(int i = 2; i < argc; i++) {
		char* text = nullptr;
		size_t size = 0;
		if(argv[i][0] == '-') {// Found flag switch
			int len = strlen(argv[i]);
			if(len > 1) {
				int sw_pos = i;
				for(int s = 1; s < len; s++) {
					i++;
					switch(argv[sw_pos][s]) {
						case 'm' : // Map commands
							if(i < argc) {
								LoadCommandArgsFromJson(argv[i]);
							} else {
								printf("No command map provided\n");
								return -1;
							}
						break;

					}
				}
			} else {
				printf("Missing switch option\n");
				return -1;
			}
		} else {
			// Go through each translation unit
			if(0 == OpenReadFile(argv[i], &size, &text)) {
				if(nullptr != text) {
					//printf("%s\n", text);
					//printf("map: %d\n", cmd2byte.size());

					// Turn everything into tokens
					g_Tokens.clear();
					Tokenize(text, argv[i], size);
					// Then, multiply the amount of these by the maximum size of them
					int64_t result = ToData();
					if(result == -1) {
						on_error = true;
						break;
					} else {
						g_Index += result; // Initialize and fill g_Buffer
					}
				}
				free(text);
			} else {
				printf("Couldn't read \"%s\", stop\n", argv[i]);
				on_error = true;
				break;
			}
		}
	}
	
	if(false == on_error) {
		// Fill addresses
		FillAddresses();
		// Finally, write file
		WriteBinary(argv[1], g_Index);
		if(nullptr != g_Buffer) {
			free(g_Buffer);
			g_Buffer = nullptr;
		}
	}
	return 0;
}
