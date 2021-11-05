struct {
    int a;
} a;
struct st{
    int a;
} b;
struct q{};
/*
#####1#####
[] > a
    c_int > a
    "struct" > type
[] > b
    c_int > a
    "struct" > type
[] > c
    c_int > a
    "struct" > type

#####2#####
[] > anonym
    c_int > a
    "struct" > type
anonym > a
anonym > b
anonym > c
*/