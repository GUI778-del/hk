# 第5讲：评估指标与模型诊断（Evaluation & Debugging）

讲义目标
- 掌握分类、回归、排序、生成任务的常见评估指标
- 学会使用混淆矩阵、ROC/AUC、精确召回曲线诊断模型问题
- 实践模型的错误分析与可视化技巧

核心要点
1. 常用分类指标：Accuracy、Precision、Recall、F1、AUC
2. 回归指标：MSE、RMSE、MAE、R^2
3. 多类别/不平衡数据的评估策略（macro/micro/weighted）
4. 排序指标：MAP、NDCG
5. 生成模型评估（BLEU、ROUGE、FID 等）简介
6. 模型不确定性与置信度校准（温度缩放、置信度阈值）
7. 错误分析流程：样本可视化、按类别/难度分组、查找数据问题

代码示例：混淆矩阵与分类报告（scikit-learn）
```python
from sklearn.metrics import classification_report, confusion_matrix
y_true = [...]
y_pred = [...]
print(classification_report(y_true, y_pred, digits=4))
cm = confusion_matrix(y_true, y_pred)
```

实践环节（notebooks/05-evaluation.py）
- 练习1：在 MNIST/CIFAR-10 上计算并可视化混淆矩阵，分析常见错误模式。
- 练习2：实现模型置信度校准（温度缩放），比较校准前后 ECE（Expected Calibration Error）。

诊断技巧
- 查看 per-class precision/recall，识别弱势类别
- 可视化错误样本以发现标签噪声或模型被困在过拟合细节
- 用学习曲线（训练/验证 loss）判断欠拟合/过拟合

参考
- scikit-learn metrics 文档
- “Practical Statistics for Data Scientists” 中的模型评估章节
