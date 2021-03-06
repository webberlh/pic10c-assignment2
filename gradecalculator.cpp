#include "gradecalculator.h"
#include "ui_gradecalculator.h"

GradeCalculator::GradeCalculator(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GradeCalculator)
{
    ui->setupUi(this);
}

GradeCalculator::~GradeCalculator()
{
    delete ui;
}

void GradeCalculator::compute_grade_pic10b() const {
    // Get which schema is to be used by checking the radio buttons
    bool schema_a = ui->radioButton->isChecked();
    bool schema_b = ui->radioButton_2->isChecked();

    // Pushback all the hw grades
    std::vector<int> hw_grades;
    hw_grades.push_back(ui->spinBox->value());
    hw_grades.push_back(ui->spinBox_9->value());
    hw_grades.push_back(ui->spinBox_10->value());
    hw_grades.push_back(ui->spinBox_11->value());
    hw_grades.push_back(ui->spinBox_12->value());
    hw_grades.push_back(ui->spinBox_13->value());
    hw_grades.push_back(ui->spinBox_15->value());
    hw_grades.push_back(ui->spinBox_14->value());

    // Remove the lowest hw grade
    {
        auto lowest_score_pos = hw_grades.cbegin();
        auto i = (hw_grades.cbegin()++);

        for(; i != hw_grades.cend(); ++i) {
            if(*lowest_score_pos > *i)
                lowest_score_pos = i;
        }
        hw_grades.erase(lowest_score_pos);
    }

    // Compute the overall hw grade
    double hw_overall = compute_vector_average(hw_grades);

    // Get the test grades
    int midterm_1 = ui->spinBox_16->value();
    int midterm_2 = ui->spinBox_17->value();
    int final = ui->spinBox_18->value();

    double overall_grade = 0;

    if(schema_a) {
        overall_grade = (0.25 * hw_overall) + (0.2 * (midterm_1 + midterm_2)) + (.35 * final);
        emit grade_computed(QString::number(overall_grade));
    }

    if(schema_b) {
        // Under schema 2 the lower midterm is dropped so only use the best one
        double best_midterm;
        if(midterm_1 > midterm_2)
            best_midterm = midterm_1;
        else
            best_midterm = midterm_2;
        overall_grade = (0.25 * hw_overall) + (0.3 * best_midterm) + (.44 * final);
        emit grade_computed(QString::number(overall_grade));
    }
}

void GradeCalculator::compute_grade_pic10c() const {
    // Get which schema is to be used by checking the radio buttons
    bool schema_a = ui->radioButton->isChecked();
    bool schema_b = ui->radioButton_2->isChecked();

    // Pushback all the hw grades
    std::vector<int> hw_grades;
    hw_grades.push_back(ui->spinBox->value());
    hw_grades.push_back(ui->spinBox_9->value());
    hw_grades.push_back(ui->spinBox_10->value());

    // Compute the overall hw grade
    double hw_overall = compute_vector_average(hw_grades);

    // Get the test and project grades
    int midterm_1 = ui->spinBox_16->value();
    int final = ui->spinBox_18->value();
    int final_project = ui->spinBox_19->value();

    double overall_grade = 0;

    if(schema_a) {
        overall_grade = (0.15 * hw_overall) + (0.25 * midterm_1) + (0.3 * final) + (0.35 * final_project);
        emit grade_computed(QString::number(overall_grade));
    }

    if(schema_b) {
        overall_grade = (0.15 * hw_overall) + (0.5 * final) + (0.35 * final_project);
        emit grade_computed(QString::number(overall_grade));
    }
}

void GradeCalculator::compute_grade() const {
    int class_selection = ui->comboBox->currentIndex();

    switch(class_selection) {
        case 0:
        compute_grade_pic10b();
        break;

        case 1:
        compute_grade_pic10c();
        break;

        default:
        break;
    }
}

void GradeCalculator::change_class(int num) const {
    bool flag;
    switch(num) {
        case 0:
        flag = true;
        break;

        case 1:
        flag = false;
        break;

        default:
        flag = true;
        break;
    }

    // Reset values and enable/disable widgets based on class
    ui->horizontalSlider_11->setEnabled(flag);
    ui->horizontalSlider_11->setValue(0);
    ui->horizontalSlider_12->setEnabled(flag);
    ui->horizontalSlider_12->setValue(0);
    ui->horizontalSlider_13->setEnabled(flag);
    ui->horizontalSlider_13->setValue(0);
    ui->horizontalSlider_14->setEnabled(flag);
    ui->horizontalSlider_14->setValue(0);
    ui->horizontalSlider_15->setEnabled(flag);
    ui->horizontalSlider_15->setValue(0);
    ui->horizontalSlider_17->setEnabled(flag);
    ui->horizontalSlider_17->setValue(0);
    ui->horizontalSlider_19->setEnabled(!flag);
    ui->horizontalSlider_19->setValue(0);

    ui->spinBox_11->setEnabled(flag);
    ui->spinBox_12->setEnabled(flag);
    ui->spinBox_13->setEnabled(flag);
    ui->spinBox_14->setEnabled(flag);
    ui->spinBox_15->setEnabled(flag);
    ui->spinBox_17->setEnabled(flag);
    ui->spinBox_19->setEnabled(!flag);

    ui->label_4->setEnabled(flag);
    ui->label_5->setEnabled(flag);
    ui->label_6->setEnabled(flag);
    ui->label_7->setEnabled(flag);
    ui->label_8->setEnabled(flag);
    ui->label_13->setEnabled(!flag);
}

double compute_vector_average(const std::vector<int>& v) {
    double average = 0;
    for(size_t i = 0; i < v.size(); ++i) {
        average += v[i];
    }
    average /= v.size();
    return average;
}
