# 第7讲：迁移学习与预训练模型实战（Transfer Learning）

讲义目标
- 理解迁移学习的动机与实践方法
- 掌握在 PyTorch 中加载、冻结/解冻层以及微调技巧
- 学会处理小样本场景与领域自适应的基本策略

核心要点
1. 迁移学习类型：特征提取（固定 backbone） vs 全量微调
2. 冻结层/分组学习率策略（layer-wise lr）
3. 数据增强与正则化在迁移学习中的作用
4. 小样本学习技巧：cosine lr、重采样、数据增强策略
5. 域适应/领域自适应简介（fine-tune +自监督等）

实践示例（notebooks/07-transfer-learning.py）
- 使用 ImageNet 预训练模型在自定义数据集上进行迁移学习
- 尝试冻结 backbone 只训练头部，然后逐步解冻

代码片段：分组参数与不同学习率
```python
backbone_params = []
head_params = []
for name, p in model.named_parameters():
    if "fc" in name or "classifier" in name:
        head_params.append(p)
    else:
        backbone_params.append(p)
optimizer = torch.optim.Adam([
    {'params': backbone_params, 'lr': 1e-5},
    {'params': head_params, 'lr': 1e-4}
])
```

练习
- 在少量标签下比较不同迁移策略（完冻结、部分解冻、全量微调），记录训练曲线与泛化性能。
- 写出迁移学习实验报告：模型选择理由、超参、效果对比、结论。

参考
- 迁移学习综述与 Hugging Face fine-tuning 指南
