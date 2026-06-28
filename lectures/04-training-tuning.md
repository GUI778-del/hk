# 第4讲：训练细节与超参数调优（Training & Hyperparameter Tuning）

讲义目标
- 掌握训练循环、优化器、常用技巧（梯度裁剪、混合精度）
- 理解超参数（学习率、批大小、权重衰减等）对训练的影响
- 学习常用调参流程与自动化方法（网格搜索、贝叶斯优化、学习率调度）

核心要点
1. 训练循环复盘（train/val/test 划分、模式切换 model.train()/eval()）
2. 损失函数选择（CrossEntropy、MSE、BCEWithLogits 等）
3. 优化器与学习率策略：SGD + momentum、Adam、AdamW
4. 学习率调度器：StepLR、CosineAnnealing、ReduceLROnPlateau、OneCycleLR
5. 正则化与泛化技巧：早停、dropout、weight decay、数据增强
6. 混合精度训练（PyTorch AMP）与多卡训练基础
7. 梯度裁剪与梯度累积（小显存训练技巧）

代码片段：使用 AMP 与 OneCycleLR
```python
from torch.cuda.amp import autocast, GradScaler
scaler = GradScaler()
scheduler = torch.optim.lr_scheduler.OneCycleLR(optimizer, max_lr=1e-3, total_steps=steps)

for epoch in range(epochs):
    model.train()
    for xb, yb in train_loader:
        xb, yb = xb.to(device), yb.to(device)
        optimizer.zero_grad()
        with autocast():
            pred = model(xb)
            loss = criterion(pred, yb)
        scaler.scale(loss).backward()
        scaler.step(optimizer)
        scaler.update()
        scheduler.step()
```

调参流程建议
- 从小模型快速试探学习率范围（LR finder）
- 固定除学习率外的关键超参，先调 LR，再调模型容量/正则化
- 使用验证集与早停避免过拟合
- 使用日志（TensorBoard、Weights & Biases）记录实验

练习（notebooks/04-training-tuning.py）
- 任务：在 CIFAR-10 上比较 Adam vs SGD、不同学习率调度器与混合精度的训练时间与性能。
- 提交内容：超参表、训练曲线图、最终 test 性能、结论与建议。

参考
- Leslie Smith 的 One Cycle Policy 论文与实现
- PyTorch AMP 文档
