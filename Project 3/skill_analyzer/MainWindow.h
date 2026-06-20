#pragma once
#include <QMainWindow>
#include <QComboBox>
#include <QCheckBox>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QGroupBox>
#include <vector>
#include <utility>
#include "Graph.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);

private slots:
    void onAnalyze();
    void onAttackerChanged(int index);

private:
    void setupUI();
    void populateAttackers();
    void populateTargets();
    void updateSkillCheckboxes(int attackerIdx);
    void showResult(const std::vector<SkillNode*>& skills,
                    const std::vector<BuffNode*>&  buffs,
                    bool useCrit, int targetHp);

    Graph  graph;

    // 왼쪽 패널
    QComboBox*   cbAttacker;
    QComboBox*   cbTarget;
    QCheckBox*   chkCrit;

    QGroupBox*              grpSkills;
    QVBoxLayout*            skillCheckLayout;
    std::vector<std::pair<QCheckBox*, SkillNode*>> skillChecks;  // 체크박스 ↔ 스킬 노드

    QGroupBox*              grpBuffers;
    std::vector<std::pair<QCheckBox*, BuffNode*>>  buffChecks;   // 체크박스 ↔ 버프 노드

    QPushButton* btnAnalyze;

    // 오른쪽 패널 (결과)
    QLabel*      lblResultTitle;
    QLabel*      lblDamage;
    QLabel*      lblSkillDetail;
    QLabel*      lblBuffDetail;
    QLabel*      lblVerdict;

    // 데이터
    std::vector<AttackerNode*> attackers;
    std::vector<TargetNode*>   targets;
    std::vector<BuffNode*>     allBuffs;
};
