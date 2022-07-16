#include <string>

auto generate_session_token(int id) -> std::string {

    srand(time(NULL) + id + rand());
    auto token = std::string();
    for(int i = 0;i < 64;i++)
    {
            int r = rand() % 127;
            while(r < 32)
                    r = rand() % 127;
                    if (r == '\n' || r == '"' | r == '\''){
                        r = 'x';
                    }
            token.push_back(char(r));
    }
	return token;    
}
