#include "Graph.h"
#include <algorithm>

Graph::~Graph() {
    for (auto* n : nodes) delete n;
}

int Graph::addNode(Node* node) {
    node->index = static_cast<int>(nodes.size());
    nodes.push_back(node);
    adjList.emplace_back();
    return node->index;
}

void Graph::addEdge(int from, int to, EdgeType type) {
    adjList[from].push_back({to, type});
}

std::vector<AttackerNode*> Graph::getAttackers() {
    std::vector<AttackerNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::ATTACKER)
            r.push_back(static_cast<AttackerNode*>(n));
    return r;
}

std::vector<TargetNode*> Graph::getTargets() {
    std::vector<TargetNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::TARGET)
            r.push_back(static_cast<TargetNode*>(n));
    return r;
}

std::vector<SkillNode*> Graph::getSkills(int attackerIdx) {
    std::vector<SkillNode*> r;
    for (auto& e : adjList[attackerIdx])
        if (e.type == EdgeType::HAS_SKILL)
            r.push_back(static_cast<SkillNode*>(nodes[e.toIndex]));
    return r;
}

std::vector<BuffNode*> Graph::getAllBuffs() {
    std::vector<BuffNode*> r;
    for (auto* n : nodes)
        if (n->type == NodeType::BUFF)
            r.push_back(static_cast<BuffNode*>(n));
    return r;
}

// ─── 데미지 계산 ──────────────────────────────────────────
float Graph::calcDamage(const std::vector<SkillNode*>& skills,
                         const std::vector<BuffNode*>&  buffs,
                         bool useCrit,
                         bool& instakill) {
    instakill = false;
    float total = 0.0f;

    for (auto* s : skills) {
        float d = s->getDamage(useCrit);
        if (d >= 99999.0f) { instakill = true; return d; }
        total += d;
    }

    float aMult = 1.0f;  // 공격자 측 버프 (아나, 메르시, 바티스트)
    float tMult = 1.0f;  // 대상 측 디버프 (젠야타)
    for (auto* b : buffs) {
        if (b->side == BuffSide::ATTACKER_BOOST) aMult *= b->multiplier;
        else                                      tMult *= b->multiplier;
    }

    return total * aMult * tMult;
}

// ─── DFS: 스킬 부분집합 (비어있지 않은 것만) ─────────────
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

// ─── DFS: 버프 부분집합 (비어있는 것 포함) ───────────────
void Graph::dfsBuffs(std::vector<BuffNode*>& v, int start,
                      std::vector<BuffNode*>& cur,
                      std::vector<std::vector<BuffNode*>>& out) {
    out.push_back(cur);  // 현재 상태 (처음엔 빈 집합)
    for (int i = start; i < static_cast<int>(v.size()); ++i) {
        cur.push_back(v[i]);
        dfsBuffs(v, i + 1, cur, out);
        cur.pop_back();
    }
}

// ─── 원콤 가능 조합 탐색 ─────────────────────────────────
std::vector<OneShotResult> Graph::findOneShotCombinations(
    int attackerIdx, int targetIdx,
    bool useCrit,
    const std::vector<BuffNode*>& allowedBuffs)
{
    auto* target = static_cast<TargetNode*>(nodes[targetIdx]);
    auto  skills = getSkills(attackerIdx);

    // 스킬 부분집합 열거
    std::vector<std::vector<SkillNode*>> skillSets;
    std::vector<SkillNode*> curS;
    dfsSkills(skills, 0, curS, skillSets);

    // 버프 부분집합 열거
    std::vector<std::vector<BuffNode*>> buffSets;
    std::vector<BuffNode*> curB;
    auto buffsCopy = allowedBuffs;
    dfsBuffs(buffsCopy, 0, curB, buffSets);

    std::vector<OneShotResult> results;
    for (auto& sSet : skillSets) {
        for (auto& bSet : buffSets) {
            bool  kill = false;
            float dmg  = calcDamage(sSet, bSet, useCrit, kill);
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

    // 정렬: 스킬+버프 수 오름차순 → 데미지 오름차순
    std::sort(results.begin(), results.end(),
        [](const OneShotResult& a, const OneShotResult& b) {
            int cntA = static_cast<int>(a.skills.size() + a.buffs.size());
            int cntB = static_cast<int>(b.skills.size() + b.buffs.size());
            if (cntA != cntB) return cntA < cntB;
            return a.totalDamage < b.totalDamage;
        });

    return results;
}
