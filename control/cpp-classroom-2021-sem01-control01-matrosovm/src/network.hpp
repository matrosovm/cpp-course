#pragma once

#include <cstdint>

struct Post {
	char const* user;
	char const* text;
	std::uint64_t create_time;
	Post* previous;
};

Post* create_post(
    char const* user,
    char const* text,
    Post* head = nullptr
);

void destroy_posts(Post const* head);
Post* previous(Post const* post);

char const* get_user(Post const* post); 
char const* get_text(Post const* post);

Post* remove_by_content(Post* head, char const* content);
std::uint64_t timestamp(Post const* post);
Post* merge(Post* head_1, Post* head_2); 
void set_text(Post* post, char const* new_text);