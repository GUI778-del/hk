# notebooks/02-transfer-learning.py
# %%
# Title: 迁移学习示例（Jupytext）
# %%
import torch
from torchvision import models, transforms, datasets
from torch.utils.data import DataLoader
from torch import nn, optim

device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

transform = transforms.Compose([
    transforms.Resize((224,224)),
    transforms.ToTensor(),
    transforms.Normalize([0.485,0.456,0.406],[0.229,0.224,0.225])
])

train_ds = datasets.FakeData(transform=transform)
train_loader = DataLoader(train_ds, batch_size=32)

model = models.resnet18(pretrained=True)
num_features = model.fc.in_features
model.fc = nn.Linear(num_features, 10)
model = model.to(device)

# 冻结 backbone
for name, param in model.named_parameters():
    if 'fc' not in name:
        param.requires_grad = False

optimizer = optim.Adam(filter(lambda p: p.requires_grad, model.parameters()), lr=1e-4)
criterion = nn.CrossEntropyLoss()

# 训练一个 epoch 示例
model.train()
for xb, yb in train_loader:
    xb, yb = xb.to(device), yb.to(device)
    pred = model(xb)
    loss = criterion(pred, yb)
    optimizer.zero_grad()
    loss.backward()
    optimizer.step()

print('one epoch done')
