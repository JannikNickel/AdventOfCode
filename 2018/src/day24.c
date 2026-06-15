#include "solutions.h"
#include "common.h"

typedef enum
{
	IMMUNE,
	INFECTION
} team;

typedef struct
{
	char data[64];
} dmg_type;

typedef struct
{
	size_t id;
	team team;
	int units;
	int hp;
	int initiative;
	int damage;
	dmg_type dmg_type;
	vector immunities;
	vector weaknesses;
	size_t curr_target_id;
} group;

typedef struct
{
	size_t id;
	team team;
} group_pred_data;

static bool simulate_round(vector* groups, int* immune_units, int* infection_units);
static int effective_damage(const group* attacker, const group* defender);
static int effective_power(const group* g);
static int groups_order_power(const void* a, const void* b);
static int groups_order_initiative(const void* a, const void* b);
static bool group_by_id(size_t index, const void* element, const void* ctx);
static void free_group(void* group);
static vector parse_input(const input* input);
static void parse_groups(const string* lines, size_t line_count, vector* groups, team t);

result day24_part1(const input* input)
{
	vector groups = parse_input(input);
	int immune_units = 0;
	int infection_units = 0;
	while(simulate_round(&groups, &immune_units, &infection_units)) { }

	vector_delete(&groups, free_group);
	return result_int(immune_units + infection_units);
}

result day24_part2(const input* input)
{
	vector groups = parse_input(input);
	int result = 0;

	int low = 0;
	int high = 50000;
	while(low <= high)
	{
		int mid = low + (high - low) / 2;

		vector groups_copy = vector_clone(&groups, NULL);
		for(size_t i = 0; i < groups_copy.size; i++)
		{
			group* g = vector_at(&groups_copy, i);
			if(g->team == IMMUNE)
			{
				g->damage += mid;
			}
		}

		int immune_units = 0;
		int infection_units = 0;
		while(simulate_round(&groups_copy, &immune_units, &infection_units)) {}

		if(immune_units > 0 && infection_units == 0)
		{
			result = immune_units;
			high = mid - 1;
		}
		else
		{
			low = mid + 1;
		}
		vector_delete(&groups_copy, NULL);
	}

	vector_delete(&groups, free_group);
	return result_int(result);
}

bool simulate_round(vector* groups, int* immune_units, int* infection_units)
{
	bool* picked = calloc(groups->size, sizeof(bool));
	vector_sort(groups, groups_order_power);

	for(size_t i = 0; i < groups->size; i++)
	{
		group* g = vector_at(groups, i);
		g->curr_target_id = SIZE_MAX;

		if(g->units == 0)
		{
			continue;
		}

		size_t best_target_index = SIZE_MAX;
		int best_damage = 0;
		int best_power = 0;
		int best_initiative = 0;
		for(size_t k = 0; k < groups->size; k++)
		{
			if(i == k)
			{
				continue;
			}

			group* target = vector_at(groups, k);
			if(target->team == g->team || target->units == 0 || picked[k])
			{
				continue;
			}

			int damage = effective_damage(g, target);
			if(damage == 0)
			{
				continue;
			}

			int target_power = effective_power(target);
			if(damage > best_damage || (damage == best_damage && target_power > best_power) || (damage == best_damage && target_power == best_power && target->initiative > best_initiative))
			{
				best_target_index = k;
				best_damage = damage;
				best_power = target_power;
				best_initiative = target->initiative;
			}
		}

		if(best_target_index != SIZE_MAX)
		{
			g->curr_target_id = ((group*)vector_at(groups, best_target_index))->id;
			picked[best_target_index] = true;
		}
	}

	vector_sort(groups, groups_order_initiative);
	bool any_units_killed = false;
	for(size_t i = 0; i < groups->size; i++)
	{
		group* g = vector_at(groups, i);
		if(g->curr_target_id == SIZE_MAX || g->units == 0)
		{
			continue;
		}

		group_pred_data pred_data = { .id = g->curr_target_id, .team = g->team == IMMUNE ? INFECTION : IMMUNE };
		group* target = vector_find(groups, group_by_id, &pred_data);
		int damage = effective_damage(g, target);
		int units_killed = damage / target->hp;
		if(units_killed > target->units)
		{
			units_killed = target->units;
		}
		target->units -= units_killed;
		any_units_killed |= units_killed > 0;
	}

	*immune_units = 0;
	*infection_units = 0;
	for(size_t i = 0; i < groups->size; i++)
	{
		group* g = vector_at(groups, i);
		if(g->team == IMMUNE)
		{
			*immune_units += g->units;
		}
		else
		{
			*infection_units += g->units;
		}
	}

	free(picked);
	return *immune_units > 0 && *infection_units > 0 && any_units_killed;
}

int effective_damage(const group* attacker, const group* defender)
{
	int damage = effective_power(attacker);
	if(vector_index_of(&defender->immunities, &attacker->dmg_type) != SIZE_MAX)
	{
		damage = 0;
	}
	else if(vector_index_of(&defender->weaknesses, &attacker->dmg_type) != SIZE_MAX)
	{
		damage *= 2;
	}
	return damage;
}

int effective_power(const group* g)
{
	return g->units * g->damage;
}

int groups_order_power(const void* a, const void* b)
{
	const group* g1 = a;
	const group* g2 = b;
	int power1 = effective_power(g1);
	int power2 = effective_power(g2);
	if(power1 != power2)
	{
		return power2 - power1;
	}
	return g2->initiative - g1->initiative;
}

int groups_order_initiative(const void* a, const void* b)
{
	const group* g1 = a;
	const group* g2 = b;
	return g2->initiative - g1->initiative;
}

bool group_by_id(size_t index, const void* element, const void* ctx)
{
	const group* g = element;
	const group_pred_data* id = ctx;
	return g->id == id->id && g->team == id->team;
}

void free_group(void* grp)
{
	group* g = grp;
	vector_delete(&g->immunities, NULL);
	vector_delete(&g->weaknesses, NULL);
}

vector parse_input(const input* input)
{
	size_t split = 0;
	while(split < input->line_count && input->lines_c[split].length != 0)
	{
		split++;
	}
	vector groups = vector_create(sizeof(group));
	parse_groups(input->lines_c, split, &groups, IMMUNE);
	parse_groups(input->lines_c + split + 1, input->line_count - split - 1, &groups, INFECTION);
	return groups;
}

void parse_groups(const string* lines, size_t line_count, vector* groups, team t)
{
	for(size_t i = 1; i < line_count; i++)
	{
		const char* line = lines[i].data;

		group g = { 0 };
		g.id = i;
		g.team = t;
		g.immunities = vector_create(sizeof(dmg_type));
		g.weaknesses = vector_create(sizeof(dmg_type));
		sscanf(line, "%d units each with %d hit points", &g.units, &g.hp);

		const char* attack_str = strstr(line, "attack that does ");
		if(attack_str)
		{
			attack_str += strlen("attack that does ");
			sscanf(attack_str, "%d %s damage at initiative %d", &g.damage, g.dmg_type.data, &g.initiative);
		}

		const char* paren_str = strchr(line, '(');
		const char* paren_end = strchr(line, ')');
		if(paren_str && paren_end)
		{
			paren_str++;
			bool weak = memcmp(paren_str, "weak to ", strlen("weak to ")) == 0;
			paren_str += weak ? strlen("weak to ") : strlen("immune to ");
			while(paren_str < paren_end)
			{				
				dmg_type el_type = { 0 };
				sscanf(paren_str, "%63[^,;)]", el_type.data);
				vector_push((weak ? &g.weaknesses : &g.immunities), &el_type);
				paren_str += strlen(el_type.data);
				if(*paren_str == ',' || *paren_str == ';')
				{
					if(*paren_str == ';')
					{
						weak = !weak;
						paren_str += 1 + strlen(weak ? "weak to " : "immune to ");
					}

					paren_str++;
					while(*paren_str == ' ')
					{
						paren_str++;
					}
				}
			}
		}

		vector_push(groups, &g);
	}
}
