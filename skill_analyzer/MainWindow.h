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
    void onAnalyze();          // 분석 버튼 클릭
    void onAttackerChanged(int index); // 공격자 변경

private:
    void setupUI();
    void populateAttackers();
    void populateTargets();
    void updateSkillCheckboxes(int attackerIdx); // 스킬 체크박스 동적 갱신
    void showResult(const std::vector<SkillNode*>& skills,
                    const std::vector<BuffNode*>&  buffs,
                    bool useCrit, int targetHp);

    Graph graph;

    // 좌측 조작 패널
    QComboBox*   cbAttacker;
    QComboBox*   cbTarget;
    QCheckBox*   chkCrit;

    QGroupBox*   grpSkills;
    QVBoxLayout* skillCheckLayout;
    std::vector<std::pair<QCheckBox*, SkillNode*>> skillChecks; // 체크박스 ↔ 스킬

    QGroupBox*   grpBuffers;
    std::vector<std::pair<QCheckBox*, BuffNode*>>  buffChecks;  // 체크박스 ↔ 버프

    QPushButton* btnAnalyze;

    // 우측 결과 패널
    QLabel* lblResultTitle;
    QLabel* lblVerdict;     // 원콤 가능/불가능
    QLabel* lblDamage;      // 총 데미지
    QLabel* lblSkillDetail; // 스킬 내역
    QLabel* lblBuffDetail;  // 버프 내역

    // 데이터
    std::vector<AttackerNode*> attackers;
    std::vector<TargetNode*>   targets;
    std::vector<BuffNode*>     allBuffs;
};
