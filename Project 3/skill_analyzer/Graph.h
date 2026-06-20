#pragma once
#include "Node.h"
#include <vector>

struct OneShotResult {
    std::vector<SkillNode*> skills;
    std::vector<BuffNode*>  buffs;
    float totalDamage = 0.0f;
    bool  instakill   = false;
};

class Graph {
public:
    ~Graph();

    int  addNode(Node* node);
    void addEdge(int from, int to, EdgeType type);

    std::vector<AttackerNode*> getAttackers();
    std::vector<TargetNode*>   getTargets();
    std::vector<SkillNode*>    getSkills(int attackerIdx);
    std::vector<BuffNode*>     getAllBuffs();

    // allowedBuffs: UI에서 체크된 버프 노드들
    std::vector<OneShotResult> findOneShotCombinations(
        int attackerIdx,
        int targetIdx,
        bool useCrit,
        const std::vector<BuffNode*>& allowedBuffs
    );

private:
    std::vector<Node*>                   nodes;
    std::vector<std::vector<GraphEdge>>  adjList;

    // DFS 부분집합 열거
    void dfsSkills(std::vector<SkillNode*>& v, int start,
                   std::vector<SkillNode*>& cur,
                   std::vector<std::vector<SkillNode*>>& out);

    void dfsBuffs(std::vector<BuffNode*>& v, int start,
                  std::vector<BuffNode*>& cur,
                  std::vector<std::vector<BuffNode*>>& out);

    float calcDamage(const std::vector<SkillNode*>& skills,
                     const std::vector<BuffNode*>&  buffs,
                     bool useCrit,
                     bool& instakill);
};
