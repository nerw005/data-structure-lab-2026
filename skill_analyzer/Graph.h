#pragma once
#include "Node.h"
#include <vector>

// 원콤 분석 결과 저장 구조체
struct OneShotResult {
    std::vector<SkillNode*> skills;
    std::vector<BuffNode*>  buffs;
    float totalDamage = 0.0f;
    bool  instakill   = false;
};

// 그래프 자료구조 - 인접 리스트 방식
class Graph {
public:
    ~Graph();

    int  addNode(Node* node);
    void addEdge(int from, int to, EdgeType type);

    std::vector<AttackerNode*> getAttackers();
    std::vector<TargetNode*>   getTargets();
    std::vector<SkillNode*>    getSkills(int attackerIdx);  // 공격자의 스킬 목록
    std::vector<BuffNode*>     getAllBuffs();

    // 선택한 스킬+버프 조합의 원콤 가능 여부 계산
    std::vector<OneShotResult> findOneShotCombinations(
        int attackerIdx,
        int targetIdx,
        bool useCrit,
        const std::vector<BuffNode*>& allowedBuffs
    );

private:
    std::vector<Node*>                   nodes;
    std::vector<std::vector<GraphEdge>>  adjList; // 인접 리스트

    // DFS로 스킬 부분집합 열거
    void dfsSkills(std::vector<SkillNode*>& v, int start,
                   std::vector<SkillNode*>& cur,
                   std::vector<std::vector<SkillNode*>>& out);

    // DFS로 버프 부분집합 열거
    void dfsBuffs(std::vector<BuffNode*>& v, int start,
                  std::vector<BuffNode*>& cur,
                  std::vector<std::vector<BuffNode*>>& out);

    // 스킬+버프 조합의 최종 데미지 계산
    float calcDamage(const std::vector<SkillNode*>& skills,
                     const std::vector<BuffNode*>&  buffs,
                     bool useCrit,
                     bool& instakill);
};
