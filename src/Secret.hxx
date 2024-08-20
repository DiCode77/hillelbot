class Secret {
public:
    Secret(const std::string& filename);
    bool is_good();
    std::string get_secret();
private:
    std::string filename;
    std::string secret;
    bool good;
};