# 第2讲：PyTorch 基础

内容速览
- Tensor 基本操作（创建、切片、运算）
- 自动求导（autograd）
- nn.Module 与模型组织方式
- DataLoader 与 Dataset（如何自定义）
- 优化器与训练循环（训练/验证/保存模型）

示例代码片段（训练循环伪代码）
```python
for epoch in range(epochs):
    model.train()
    for xb, yb in train_loader:
        pred = model(xb)
        loss = criterion(pred, yb)
        optimizer.zero_grad()
        loss.backward()
        optimizer.step()
```
