# notebooks/03-data-augmentation.py
# %%
# Title: 数据增强实验
# %%
from torchvision import transforms, datasets
from torch.utils.data import DataLoader

transform_aug = transforms.Compose([
    transforms.RandomResizedCrop(32),
    transforms.RandomHorizontalFlip(),
    transforms.ToTensor()
])

transform_base = transforms.Compose([
    transforms.Resize((32,32)),
    transforms.ToTensor()
])

train_aug = datasets.FakeData(transform=transform_aug)
train_base = datasets.FakeData(transform=transform_base)

loader_aug = DataLoader(train_aug, batch_size=64)
loader_base = DataLoader(train_base, batch_size=64)

print('datasets ready')
