#pragma once
#include "Graph.h"

class DataLoader {
public:
    static void init(Graph& graph);

private:
    static SkillNode* makeSkill(
        const std::string& name,
        SkillCategory cat,
        int base, int crit = 0,
        int proj = 1,
        bool instakill = false,
        bool perSecond = false,
        int initDmg = 0,
        int perSecDmg = 0);
};
