#include <cstdio>

int main() {
    const char *s = "#include <cstdio>\n\nint main() {\n    const char *s = %c%s%c;\n    std::printf(s, 34, s, 34);\n    return 0;\n}\n";
    std::printf(s, 34, s, 34);
    return 0;
}
