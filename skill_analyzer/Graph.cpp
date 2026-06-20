#include "Graph.h"
#include <algorithm>

Graph::~Graph() {
    for (auto* n : nodes) delete n;
}

// 노드 추가 후 인덱스 반환
int Graph::addNode(Node* node) {
    node->index = static_cast<int>(nodes.size());
    nodes.push_back(node);
    adjList.emplace_back();
    return node->index;
}

// 방향 엣지 추가
void Graph::addEdge(int from, int to, EdgeType type) {
    adjList[from].push_back({to, type});
}

// ATTACKER 타입 노드만 수집
std::vector<AttackerNode*> Graph::getAttackers() {
    std::vector<AttackerNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::ATTACKER)
            r.push_back(static_cast<AttackerNode*>(n));
    return r;
}

// TARGET 타입 노드만 수집
std::vector<TargetNode*> Graph::getTargets() {
    std::vector<TargetNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::TARGET)
            r.push_back(static_cast<TargetNode*>(n));
    return r;
}

// 공격자 노드에서 HAS_SKILL 엣지를 따라 스킬 목록 반환
std::vector<SkillNode*> Graph::getSkills(int attackerIdx) {
    std::vector<SkillNode*> r;
    for (auto& e : adjList[attackerIdx])
        if (e.type == EdgeType::HAS_SKILL)
            r.push_back(static_cast<SkillNode*>(nodes[e.toIndex]));
    return r;
}

// BUFF 타입 노드 전체 반환
std::vector<BuffNode*> Graph::getAllBuffs() {
    std::vector<BuffNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::BUFF)
            r.push_back(static_cast<BuffNode*>(n));
    return r;
}

// 공격자/피격자 버프를 분리해서 곱계산 후 최종 데미지 반환
float Graph::calcDamage(const std::vector<SkillNode*>& skills,
                         const std::vector<BuffNode*>&  buffs,
                         bool useCrit, bool& instakill) {
    instakill = false;
    float total = 0.0f;

    for (auto* s : skills) {
        float d = s->getDamage(useCrit);
        if (d >= 99999.0f) { instakill = true; return d; }
        total += d;
    }

    float aMult = 1.0f; // 공격자 측 배율 (아나, 메르시, 바티스트)
    float tMult = 1.0f; // 피격자 측 배율 (젠야타)
    for (auto* b : buffs) {
        if (b->side == BuffSide::ATTACKER_BOOST) aMult *= b->multiplier;
        else                                      tMult *= b->multiplier;
    }

    return total * aMult * tMult;
}

// DFS - 스킬 비어있지 않은 부분집합 열거
void Graph::dfsSkills(std::vector<SkillNode*>& v, int start,
                       std::vector<SkillNode*>& cur,
                       std::vector<std::vector<SkillNode*>>& out) {
    if (!cur.empty()) out.push_back(cur);
    for (int i = start; i < static_cast<int>(v.size()); ++i) {
        cur.push_back(v[i]);
        dfsSkills(v, i + 1, cur, out);
        cur.pop_back();
    }
}

// DFS - 버프 전체 부분집합 열거 (빈 집합 포함)
void Graph::dfsBuffs(std::vector<BuffNode*>& v, int start,
                      std::vector<BuffNode*>& cur,
                      std::vector<std::vector<BuffNode*>>& out) {
    out.push_back(cur);
    for (int i = start; i < static_cast<int>(v.size()); ++i) {
        cur.push_back(v[i]);
        dfsBuffs(v, i + 1, cur, out);
        cur.pop_back();
    }
}

// 선택된 스킬+버프 조합에서 원콤 가능한 경우만 추려서 반환
std::vector<OneShotResult> Graph::findOneShotCombinations(
    int attackerIdx, int targetIdx,
    bool useCrit,
    const std::vector<BuffNode*>& allowedBuffs)
{
    auto* target = static_cast<TargetNode*>(nodes[targetIdx]);
    auto  skills = getSkills(attackerIdx);

    std::vector<std::vector<SkillNode*>> skillSets;
    std::vector<SkillNode*> curS;
    dfsSkills(skills, 0, curS, skillSets);

    std::vector<std::vector<BuffNode*>> buffSets;
    std::vector<BuffNode*> curB;
    auto buffsCopy = allowedBuffs;
    dfsBuffs(buffsCopy, 0, curB, buffSets);

    std::vector<OneShotResult> results;
    for (auto& sSet : skillSets) {
        for (auto& bSet : buffSets) {
            bool  kill = false;
            float dmg  = calcDamage(sSet, bSet, useCrit, kill);
            // 대상 체력 이상이면 원콤 가능
            if (kill || dmg >= static_cast<float>(target->hp)) {
                OneShotResult r;
                r.skills      = sSet;
                r.buffs       = bSet;
                r.totalDamage = dmg;
                r.instakill   = kill;
                results.push_back(r);
            }
        }
    }

    // 스킬+버프 수 적은 순으로 정렬
    std::sort(results.begin(), results.end(),
        [](const OneShotResult& a, const OneShotResult& b) {
            int cntA = static_cast<int>(a.skills.size() + a.buffs.size());
            int cntB = static_cast<int>(b.skills.size() + b.buffs.size());
            if (cntA != cntB) return cntA < cntB;
            return a.totalDamage < b.totalDamage;
        });

    return results;
}
