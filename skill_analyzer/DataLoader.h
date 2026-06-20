#pragma once
#include "Graph.h"

// 캐릭터 데이터를 그래프에 초기화하는 클래스
class DataLoader {
public:
    static void init(Graph& graph);

private:
    // SkillNode 생성 헬퍼
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
