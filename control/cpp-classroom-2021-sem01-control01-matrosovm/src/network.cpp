#include "network.hpp"

#include <cstring>

#define TASK_3

#ifdef TASK_3
static std::uint64_t get_current_time() {
  static std::uint64_t current_time = 0;
  return current_time++;
}

#endif // TASK_3
#undef TASK_3

Post* create_post(
    char const* user,
    char const* text,
    Post* head
) {
	Post* post = new Post();
	
	char* tmp1 = new char[strlen(user) + 1]();
	strcpy(tmp1, user);
	post->user = tmp1;	

	char* tmp2 = new char[strlen(text) + 1]();
	strcpy(tmp2, text);
	post->text = tmp2;

	post->previous = head;
	post->create_time = get_current_time();

	return post;
}

void destroy_posts(Post const* head) {
	while (head) {
		Post* tmp = head->previous;
		delete[] head->user;
		delete[] head->text;
		delete head;
		head = nullptr;

		head = tmp;
	}
}

Post* previous(Post const* post) {
	if (post)
		return post->previous;
	return nullptr;
}

char const* get_user(Post const* post) {
	if (post)
		return post->user;
	return nullptr;
} 
char const* get_text(Post const* post) {
	if (post)
		return post->text;
	return nullptr;
}


Post* remove_by_content(Post* head, char const* content) {
	Post* new_head = nullptr;
	bool is_first = true;

	while (head) {
		Post* tmp = head->previous;
		if (head->text && std::strstr(head->text, content)) {
			delete[] head->user;
			delete[] head->text;
			delete head;
			head = nullptr;
		} else if (is_first) {
			new_head = head;
			is_first = false;
		}
		head = tmp;
	}

	return new_head;
}

std::uint64_t timestamp(Post const* post) {
	if (post)
		return post->create_time;
	return 0;
}

Post* merge(Post* head_1, Post* head_2) {
	Post* tmp = nullptr;

	while (head_1 && head_2) {
		if (head_1->create_time < head_2->create_time) {
			create_post(head_1->user, head_1->text, tmp);
			head_1 = head_1->previous;
		} else {
			create_post(head_2->user, head_2->text, tmp);
			head_2 = head_2->previous;
		}
	}
	if (head_1) {
		create_post(head_1->user, head_1->text, tmp);
		head_1 = head_1->previous;
	} else {
		create_post(head_2->user, head_2->text, tmp);
		head_2 = head_2->previous;	
	}

	return tmp;
}

void set_text(Post* post, char const* new_text) {
	char* tmp = new char[strlen(new_text) + 1]();
	strcpy(tmp, new_text);
	delete[] post->text;
	post->text = new_text;	
}

