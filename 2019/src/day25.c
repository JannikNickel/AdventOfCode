#include "solutions.h"
#include "common.h"
#include "shared/intcode.h"

typedef struct room
{
	string name;
	string* item;
	vector connected;
} room;

typedef struct
{
	room* room;
	int direction;
} connection;

typedef struct
{
	vector code;
	vector input;
	size_t inst_ptr;
	size_t rel_base;
	room* prev_room;
	int direction;
} state;

static const char* DIRS[4] = { "north", "south", "east", "west" };
static const int INV_DIRS[4] = { 1, 0, 3, 2 };

static bool pickup_all(state* state, room* root, string* target, int* target_dir);
static void write_str_cmd(vector* input, const char* prefix, const string* str);
static void write_move_cmd(vector* input, int dir);
static bool is_space_or_newline(char c);
static size_t str_hsh(const string* a);
static bool str_eql(const string* a, const string* b);

result day25_part1(const input* input)
{
	vector code = intcode_parse(input->raw);
	vector states = vector_create(sizeof(state));
	vector all_rooms = vector_create(sizeof(room*));
	vector_push(&states, &(state) { .code = vector_clone(&code, NULL), .input = vector_create(sizeof(int64_t)), .inst_ptr = 0, .rel_base = 0, .prev_room = NULL, .direction = -1 });
	set closed = set_create(sizeof(string), 128, 0.75f, str_hsh, str_eql);

	room* root = NULL;
	while(states.size > 0)
	{
		state s = *(state*)vector_first(&states);
		vector_remove_first(&states, NULL);

		vector out = vector_create(sizeof(int64_t));
		char* output_str = NULL;
		intcode_run_result res = intcode_continue(&s.code, &s.input, &out, s.inst_ptr, s.rel_base, false, false);

		if(out.size > 0)
		{
			output_str = malloc(sizeof(char) * out.size + 1);
			output_str[out.size] = '\0';
			for(size_t i = 0; i < out.size; i++)
			{
				output_str[i] = (char)*(int64_t*)vector_at(&out, i);
			}
			string str = string_wrap(output_str);
			size_t room_start = string_find_cstr(str, "== ", 0);
			room* r = NULL;
			if(room_start != (size_t)(-1))
			{
				room_start += 3;
				size_t room_end = string_find_cstr(str, " ==", room_start);
				string room_name = string_sub(str, room_start, room_end - room_start);

				if(!set_insert(&closed, &room_name))
				{
					string_delete(&room_name);
					goto CLEANUP;
				}

				r = malloc(sizeof(room));
				r->name = room_name;
				r->item = NULL;
				r->connected = vector_create(sizeof(connection));
				vector_push(&all_rooms, &r);

				const char* items_prefix = "Items here:\n";
				size_t items_start = string_find_cstr(str, items_prefix, 0);
				if(items_start != (size_t)(-1))
				{
					items_start += strlen(items_prefix) + 2;
					string item = string_sub(str, items_start, string_find_char(str, '\n', items_start) - items_start);
					string* item_ptr = malloc(sizeof(string));
					memcpy(item_ptr, &item, sizeof(string));
					r->item = item_ptr;
				}

				if(root == NULL)
				{
					root = r;
				}
				if(s.prev_room != NULL)
				{
					connection c = (connection) { .room = r, .direction = s.direction };
					vector_push(&s.prev_room->connected, &c);
				}
			}
			else if(strcmp(output_str, "You can't go that way"))
			{
				goto CLEANUP;
			}

			for(int i = 0; i < 4; i++)
			{
				state next;
				next.inst_ptr = res.inst_ptr;
				next.rel_base = res.rel_base;
				next.code = vector_clone(&s.code, NULL);
				next.input = vector_create(sizeof(int64_t));
				write_move_cmd(&next.input, i);
				next.prev_room = r;
				next.direction = i;
				vector_push(&states, &next);
			}
		}

		CLEANUP:
		free(output_str);
		vector_delete(&s.code, NULL);
		vector_delete(&s.input, NULL);
		vector_delete(&out, NULL);
	}

	state root_state = (state) { .code = code, .input = vector_create(sizeof(int64_t)), .inst_ptr = 0, .rel_base = 0, .direction = 0, .prev_room = NULL };
	intcode_run_result res = intcode_continue(&root_state.code, &root_state.input, NULL, root_state.inst_ptr, root_state.rel_base, false, false);
	root_state.inst_ptr = res.inst_ptr;
	root_state.rel_base = res.rel_base;
	pickup_all(&root_state, root, NULL, NULL);

	string target = string_from("Security Checkpoint");
	int final_dir = 0;
	pickup_all(&root_state, root, &target, &final_dir);
	string_delete(&target);

	write_str_cmd(&root_state.input, "inv", NULL);
	vector output = vector_create(sizeof(int64_t));
	intcode_continue(&root_state.code, &root_state.input, &output, root_state.inst_ptr, root_state.rel_base, false, false);
	vector items = vector_create(sizeof(string));
	for(size_t i = 0; i < output.size; i++)
	{
		char c = (char)*(int64_t*)vector_at(&output, i);
		if(c == '-')
		{
			i += 2;
			string item = string_empty();
			while((c = *(int64_t*)vector_at(&output, i)) != '\n')
			{
				string_append_char(&item, c);
				i++;
			}
			vector_push(&items, &item);
		}
	}
	vector_delete(&output, NULL);

	uint64_t result = 0;
	while(true)
	{
		vector code_cpy = vector_clone(&root_state.code, NULL);
		vector inp = vector_create(sizeof(int64_t));
		vector out = vector_create(sizeof(int64_t));
		size_t inst_ptr = root_state.inst_ptr;
		size_t rel_base = root_state.rel_base;

		for(size_t i = 0; i < items.size; i++)
		{
			string* item = vector_at(&items, i);
			if(rand() < RAND_MAX / 2)
			{
				vector inp = vector_create(sizeof(int64_t));
				write_str_cmd(&inp, "drop ", item);
				intcode_run_result res = intcode_continue(&code_cpy, &inp, NULL, inst_ptr, rel_base, false, false);
				inst_ptr = res.inst_ptr;
				rel_base = res.rel_base;
			}
		}

		write_move_cmd(&inp, final_dir);
		intcode_run_result res = intcode_continue(&code_cpy, &inp, &out, inst_ptr, rel_base, false, false);

		char* output_str = malloc(sizeof(char) * out.size + 1);
		output_str[out.size] = '\0';
		for(size_t i = 0; i < out.size; i++)
		{
			output_str[i] = (char)*(int64_t*)vector_at(&out, i);
		}
		if(string_find_cstr(string_wrap(output_str), "Alert!", 0) == (size_t)(-1))
		{
			const char* res_prefix = "typing ";
			size_t start = string_find_cstr(string_wrap(output_str), res_prefix, 0) + strlen(res_prefix);
			result = strtoull(output_str + start, NULL, 10);
			break;
		}
		free(output_str);
	}

	for(size_t i = 0; i < all_rooms.size; i++)
	{
		room* r = *(room**)vector_at(&all_rooms, i);
		string_delete(&r->name);
		string_delete(r->item);
		free(r->item);
		vector_delete(&r->connected, NULL);
	}
	vector_delete(&all_rooms, NULL);
	vector_delete(&root_state.code, NULL);
	vector_delete(&root_state.input, NULL);
	vector_delete(&states, NULL);
	vector_delete(&items, NULL);
	set_delete(&closed, NULL);
	return result_uint(result);
}

result day25_part2(const input* input)
{
	//Day 25 only has 1 puzzle
	return result_none();
}

static bool pickup_all(state* state, room* root, string* target, int* target_dir)
{
	if(root->item != NULL)
	{
		if(strcmp(root->item->data, "giant electromagnet") != 0
			&& strcmp(root->item->data, "infinite loop") != 0
			&& strcmp(root->item->data, "photons") != 0
			&& strcmp(root->item->data, "escape pod") != 0
			&& strcmp(root->item->data, "molten lava") != 0)
		{
			write_str_cmd(&state->input, "take ", root->item);
			intcode_run_result res = intcode_continue(&state->code, &state->input, NULL, state->inst_ptr, state->rel_base, false, false);
			state->inst_ptr = res.inst_ptr;
			state->rel_base = res.rel_base;
		}
	}

	for(int i = 0; i < root->connected.size; i++)
	{
		connection* c = vector_at(&root->connected, i);
		room* r = c->room;

		write_move_cmd(&state->input, c->direction);
		intcode_run_result res = intcode_continue(&state->code, &state->input, NULL, state->inst_ptr, state->rel_base, false, false);
		state->inst_ptr = res.inst_ptr;
		state->rel_base = res.rel_base;

		if(target != NULL && string_equals(r->name, *target))
		{
			if(target_dir != NULL)
			{
				if(r->connected.size > 0)
				{
					*target_dir = ((connection*)vector_first(&r->connected))->direction;
				}
			}
			return false;
		}

		if(!pickup_all(state, r, target, target_dir))
		{
			return false;
		}

		write_move_cmd(&state->input, INV_DIRS[c->direction]);
		res = intcode_continue(&state->code, &state->input, NULL, state->inst_ptr, state->rel_base, false, false);
		state->inst_ptr = res.inst_ptr;
		state->rel_base = res.rel_base;
	}
	return true;
}

void write_str_cmd(vector* input, const char* prefix, const string* str)
{
	size_t prefix_len = strlen(prefix);
	for(size_t i = 0; i < prefix_len; i++)
	{
		vector_push(input, &(int64_t) { prefix[i] });
	}
	if(str != NULL)
	{
		for(size_t i = 0; i < str->length; i++)
		{
			vector_push(input, &(int64_t) { string_at(*str, i) });
		}
	}
	vector_push(input, &(int64_t) { '\n' });
}

void write_move_cmd(vector* input, int dir)
{
	int len = strlen(DIRS[dir]);
	for(int k = 0; k < len; k++)
	{
		vector_push(input, &(int64_t) { DIRS[dir][k] });
	}
	vector_push(input, &(int64_t) { '\n' });
}

bool is_space_or_newline(char c)
{
	return isspace(c) || c == '\n';
}

size_t str_hsh(const string* a)
{
	return string_hash(*a);
}

bool str_eql(const string* a, const string* b)
{
	return string_equals(*a, *b);
}
