import numpy as np
import torch
import random
import matplotlib.pyplot as plt
import torchvision.utils as tv_utils

import sys
sys.path.append('..')

from cs231n.emoji_dataset import EmojiDataset
from cs231n.gaussian_diffusion import GaussianDiffusion

def rel_error(x, y):
    """Returns relative error."""
    return np.max(np.abs(x - y) / (np.maximum(1e-10, np.abs(x) + np.abs(y))))

# First, let's load and visualize the dataset.
# Each sample of the dataset is a tuple: (image, {"text_emb": <tensor>, "text": <string>})
# We will use a pretrained text-encoder to encode text into an embedding vector.
# We have pre-encoded the dataset texts into embeddings for faster training.
image_size = 32
dataset = EmojiDataset(image_size)

def visualize_samples(dataset, num_samples=25, grid_size=(5, 5)):
    # Randomly sample indices
    indices = random.sample(range(len(dataset)), num_samples)
    samples = [dataset[i] for i in indices]

    # Inspect one sample
    img_shape = list(samples[0][0].shape)
    emb_shape = list(samples[0][1]["text_emb"].shape)
    print(f"One sample: (image: {img_shape}, {{ \"text_emb\": {emb_shape}, \"text\": string }})")

    # Extract images and texts
    images = torch.stack([sample[0] for sample in samples])  # Stack images
    texts = [sample[1]["text"] for sample in samples]  # Extract text descriptions

    # Create a grid of images
    grid_img = tv_utils.make_grid(images, nrow=grid_size[1], padding=2)

    # Convert to numpy for plotting
    grid_img = grid_img.permute(1, 2, 0).numpy()

    # Plot the images
    fig, ax = plt.subplots(figsize=(10, 10))
    ax.imshow(grid_img)
    ax.axis("off")

    # Add text annotations
    grid_w, grid_h = grid_size
    img_w, img_h = grid_img.shape[1] // grid_w, grid_img.shape[0] // grid_h

    for i, text in enumerate(texts):
        row, col = divmod(i, grid_w)
        x, y = col * img_w, row * img_h
        ax.text(x+5, y+5, text[:30], fontsize=8, color='white', bbox=dict(facecolor='black', alpha=0.5))

    plt.show()

#visualize_samples(dataset)

# Test GaussianDiffusion.q_sample method
sz = 2
b = 3

diffusion = GaussianDiffusion(
      model=None,
      image_size=sz,
      timesteps=1000,
      beta_schedule="sigmoid",
)

t = torch.tensor([0, 300, 999]).long()
x_start = torch.linspace(-0.9, 0.6, b*3*sz*sz).view(b, 3, sz, sz)
noise = torch.linspace(-0.7, 0.8, b*3*sz*sz).view(b, 3, sz, sz)
x_t = diffusion.q_sample(x_start, t, noise)

expected_x_t = np.array([
    [
        [[-0.9119949, -0.86840147], [-0.8248081, -0.7812148]],
        [[-0.7376214, -0.694028], [-0.65043473, -0.6068413]],
        [[-0.563248, -0.51965463], [-0.47606122, -0.43246788]],
    ],
    [
        [[-0.42800453, -0.37039882], [-0.31279305, -0.2551873]],
        [[-0.19758154, -0.1399758], [-0.08237009, -0.024764337]],
        [[0.032841414, 0.090447165], [0.14805292, 0.20565866]],
    ],
    [
        [[0.32864183, 0.37152246], [0.41440308, 0.45728368]],
        [[0.50016433, 0.5430449], [0.5859255, 0.6288062]],
        [[0.67168677, 0.7145674], [0.757448, 0.8003287]],
    ],
]).astype(np.float32)

# Should see zero relative error
#print("x_t error: ", rel_error(x_t.numpy(), expected_x_t))

# Let's visualize the noisy images at various timesteps.
diffusion = GaussianDiffusion(
      model=None,
      image_size=image_size,
      timesteps=1000,
)

B = 10
img = dataset[770][0]  # 3 x H x W
x_start = img[None].repeat(B, 1, 1, 1)  # B x 3 x H x W
noise = torch.randn_like(x_start)  # B x 3 x H x W
t = torch.linspace(0, 1000-1, B).long()

x_start = diffusion.normalize(x_start)
x_t = diffusion.q_sample(x_start, t, noise)
x_t = diffusion.unnormalize(x_t).clamp(0, 1)
grid_img = tv_utils.make_grid(x_t, nrow=5, padding=2)
grid_img = grid_img.permute(1, 2, 0).cpu().numpy()
fig, ax = plt.subplots(figsize=(10, 10))
ax.imshow(grid_img)
ax.axis("off")
#plt.show()

from cs231n.unet import Unet, ResnetBlock, Downsample, Upsample

# Test `predict_noise_from_start` and `predict_start_from_noise`
sz = 2
b = 3

diffusion = GaussianDiffusion(
      model=None,
      image_size=sz,
      timesteps=1000,
      beta_schedule="sigmoid",
)

t = torch.tensor([1, 300, 998]).long()
x_start = torch.linspace(-0.91, 0.67, b*3*sz*sz).view(b, 3, sz, sz)
noise = torch.linspace(-0.73, 0.81, b*3*sz*sz).view(b, 3, sz, sz)
x_t = diffusion.q_sample(x_start, t, noise)

pred_noise = diffusion.predict_noise_from_start(x_t, t, x_start)
pred_x_start = diffusion.predict_start_from_noise(x_t, t, noise)

# Should relative errors around 1e-5 or less
#print("noise error: ", rel_error(pred_noise.numpy(), noise.numpy()))
#print("x_start error: ", rel_error(pred_x_start.numpy(), x_start.numpy()))

np.random.seed(231)
torch.manual_seed(231)

dim = 4
condition_dim = 4
dim_mults = (2, 4)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)

b = 2
h = w = 4
inp_x = torch.randn(b, 3, h, w)
inp_text_emb = torch.randn(b, condition_dim)
inp_t = torch.tensor([8, 25]).long()
out = unet.forward(x=inp_x, time=inp_t,
                   model_kwargs={"text_emb": inp_text_emb}).detach().numpy()

expected_out = np.array(
      [[[[ 0.14615417,  0.36610782,  0.27948245,  0.2229169 ],
         [ 1.0268314 , -0.04441035,  0.33097324,  0.21493062],
         [ 0.15944722,  1.1060286 ,  0.36489075,  0.39395577],
         [ 0.5593624 ,  0.95084137,  0.46409354, -0.15076232]],

        [[ 0.07152754, -0.19067341,  0.36995906,  0.1898715 ],
         [ 0.18764025, -0.37758452,  0.22994985,  0.14644745],
         [ 0.39364466,  0.42091975,  0.75438905, -0.17806   ],
         [ 0.0934296 ,  0.44165182,  0.2768886 ,  0.19760622]],

        [[ 0.39873862,  0.86417544,  0.707601  ,  0.5136454 ],
         [ 0.8151177 ,  0.01816908,  0.64427924,  0.45256743],
         [ 0.6901425 ,  1.0449984 ,  0.8272561 ,  0.38516602],
         [ 0.48775655,  0.91759497,  0.56286275,  0.38452417]]],


       [[[ 0.31076878,  0.25998223,  0.35973004, -0.01464513],
         [ 0.37456402,  0.10733554,  1.1211727 ,  0.596719  ],
         [-0.19628221,  0.49115434,  0.5591996 , -0.02811927],
         [ 0.2980889 ,  0.7983323 ,  0.31545636,  0.1045265 ]],

        [[-0.21484727, -0.11434001,  0.01019827, -0.07907221],
         [-0.14186645,  0.2666731 ,  0.36379665,  0.25780094],
         [ 0.6618308 ,  0.09432775,  0.3441353 ,  0.11780772],
         [ 0.3818162 ,  0.54577625,  0.15127666,  0.2136025 ]],

        [[ 0.23299581,  0.51728034,  0.5330554 ,  0.30019608],
         [ 0.34902877,  0.29055628,  1.1447697 ,  0.5087651 ],
         [ 0.7447357 ,  0.4974355 ,  0.564866  ,  0.4631402 ],
         [ 0.6024195 ,  0.8882342 ,  0.46354175,  0.4344969 ]]]])

#print("forward error: ", rel_error(out, expected_out))


from cs231n.unet import Unet, ResnetBlock, Downsample, Upsample


"""dim = 2
condition_dim = 4
dim_mults = (1, 2, 4)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)


# Check number of layers
assert len(unet.downs) == len(dim_mults), "Number of Unet downsampling blocks is wrong."
assert len(unet.ups) == len(dim_mults), "Number of Unet upsampling blocks is wrong."


# Check layers
try:
  expected_downs_dims = [2, 2, 8, 2, 2, 8, 2, 2, 8, 2, 2, 8, 4, 4, 8, 4, 4, 8]
  downs_dims = [
      d for m in unet.downs for d in [
          m[0].dim, m[0].dim_out, m[0].context_dim, m[1].dim, m[1].dim_out, m[1].context_dim,
      ]
  ]
  assert downs_dims == expected_downs_dims, "Dimensions don't match"
except Exception as e:
  raise RuntimeError("Downsampling blocks wrongly configured") from e


try:
  expected_ups_dims = [8, 4, 8, 8, 4, 8, 4, 2, 8, 4, 2, 8, 4, 2, 8, 4, 2, 8]
  ups_dims = [
      d for m in unet.ups for d in [
          m[1].dim, m[1].dim_out, m[1].context_dim, m[2].dim, m[2].dim_out, m[2].context_dim,
      ]
  ]
  assert ups_dims == expected_ups_dims, "Dimensions don't match"
except Exception as e:
  raise RuntimeError("Upsampling blocks wrongly configured") from e

# Check number of parameters
num_params = sum(p.numel() for p in unet.parameters())
expected_num_params = 6499
assert num_params == expected_num_params, "Unet model creation is wrong!"  """

"""np.random.seed(231)
torch.manual_seed(231)

dim = 4
condition_dim = 4
dim_mults = (2, 4)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)

b = 2
h = w = 4
inp_x = torch.randn(b, 3, h, w)
inp_text_emb = torch.randn(b, condition_dim)
inp_t = torch.tensor([8, 25]).long()
out = unet.forward(x=inp_x, time=inp_t,
                   model_kwargs={"text_emb": inp_text_emb}).detach().numpy()

expected_out = np.array(
      [[[[ 0.14615417,  0.36610782,  0.27948245,  0.2229169 ],
         [ 1.0268314 , -0.04441035,  0.33097324,  0.21493062],
         [ 0.15944722,  1.1060286 ,  0.36489075,  0.39395577],
         [ 0.5593624 ,  0.95084137,  0.46409354, -0.15076232]],

        [[ 0.07152754, -0.19067341,  0.36995906,  0.1898715 ],
         [ 0.18764025, -0.37758452,  0.22994985,  0.14644745],
         [ 0.39364466,  0.42091975,  0.75438905, -0.17806   ],
         [ 0.0934296 ,  0.44165182,  0.2768886 ,  0.19760622]],

        [[ 0.39873862,  0.86417544,  0.707601  ,  0.5136454 ],
         [ 0.8151177 ,  0.01816908,  0.64427924,  0.45256743],
         [ 0.6901425 ,  1.0449984 ,  0.8272561 ,  0.38516602],
         [ 0.48775655,  0.91759497,  0.56286275,  0.38452417]]],


       [[[ 0.31076878,  0.25998223,  0.35973004, -0.01464513],
         [ 0.37456402,  0.10733554,  1.1211727 ,  0.596719  ],
         [-0.19628221,  0.49115434,  0.5591996 , -0.02811927],
         [ 0.2980889 ,  0.7983323 ,  0.31545636,  0.1045265 ]],

        [[-0.21484727, -0.11434001,  0.01019827, -0.07907221],
         [-0.14186645,  0.2666731 ,  0.36379665,  0.25780094],
         [ 0.6618308 ,  0.09432775,  0.3441353 ,  0.11780772],
         [ 0.3818162 ,  0.54577625,  0.15127666,  0.2136025 ]],

        [[ 0.23299581,  0.51728034,  0.5330554 ,  0.30019608],
         [ 0.34902877,  0.29055628,  1.1447697 ,  0.5087651 ],
         [ 0.7447357 ,  0.4974355 ,  0.564866  ,  0.4631402 ],
         [ 0.6024195 ,  0.8882342 ,  0.46354175,  0.4344969 ]]]])

print("forward error: ", rel_error(out, expected_out))"""


"""np.random.seed(231)
torch.manual_seed(231)

dim = 4
condition_dim = 4
dim_mults = (2, 4)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)

h = w = 4
b = 3
diffusion = GaussianDiffusion(
      model=unet,
      image_size=h,
      timesteps=1000,
      beta_schedule="sigmoid",
      objective="pred_x_start",
)

inp_x = torch.randn(b, 3, h, w)
inp_model_kwargs = {"text_emb": torch.randn(b, condition_dim)}
out = diffusion.p_losses(inp_x, inp_model_kwargs)
expected_out = 30.0732689

print("forward error: ", rel_error(out.item(), expected_out))"""


"""np.random.seed(231)
torch.manual_seed(231)

dim = 4
condition_dim = 4
dim_mults = (2,)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)

h = w = 4
b = 1
inp_x_t = torch.randn(b, 3, h, w)
inp_model_kwargs = {"text_emb": torch.randn(b, condition_dim)}
t = 231

# test 1
diffusion = GaussianDiffusion(
      model=unet,
      image_size=h,
      timesteps=1000,
      beta_schedule="sigmoid",
      objective="pred_x_start",
)
out = diffusion.p_sample(inp_x_t, t, inp_model_kwargs).detach().numpy()
expected_out = np.array(
    [[[[ 1.1339471 ,  0.12097352, -0.7175048 ,  1.3196243 ],
         [-0.27657282,  0.4899886 ,  1.0170169 , -0.8242867 ],
         [-0.18946372,  0.9899801 ,  0.01498353,  0.39722288],
         [-0.97995025, -0.5947938 , -0.07796463, -0.07311387]],

        [[ 0.0739838 , -1.5537696 ,  0.43128064, -0.7395982 ],
         [-1.0517508 , -1.7030833 ,  0.79073197, -1.217138  ],
         [-0.5314434 ,  0.9862699 ,  0.6568664 , -0.4559122 ],
         [-0.17322278,  0.51251256, -0.75741345, -0.3967054 ]],

        [[ 0.8546979 ,  1.6186953 ,  1.9930652 ,  0.57347   ],
         [ 0.20219846,  0.5374655 , -0.81597316,  1.9089762 ],
         [ 0.7327057 ,  1.19275   ,  1.8593936 , -1.4582647 ],
         [ 0.68447256, -0.9056745 ,  0.7863245 ,  0.14455058]]]])
print("forward error: ", rel_error(out, expected_out))

# test 2
diffusion = GaussianDiffusion(
      model=unet,
      image_size=h,
      timesteps=1000,
      beta_schedule="cosine",
      objective="pred_noise",
)
out = diffusion.p_sample(inp_x_t, t, inp_model_kwargs).detach().numpy()
expected_out = np.array(
    [[[[ 1.1036711 ,  0.08143333, -0.6856102 ,  1.3826138 ],
         [-0.25455472,  0.514572  ,  1.104592  , -0.75972646],
         [-0.22729763,  0.9837706 ,  0.05891411,  0.52049375],
         [-1.0331786 , -0.5416254 , -0.01623197, -0.04838388]],

        [[ 0.08324978, -1.545468  ,  0.41357145, -0.63511896],
         [-1.1362139 , -1.7128816 ,  0.8694859 , -1.2297069 ],
         [-0.49168122,  1.0043695 ,  0.6759953 , -0.5297671 ],
         [-0.10931232,  0.52347076, -0.80946106, -0.5015002 ]],

        [[ 0.7437265 ,  1.590004  ,  1.9481117 ,  0.5656144 ],
         [ 0.22895451,  0.5289113 , -0.8511001 ,  1.8864397 ],
         [ 0.72863096,  1.2271638 ,  1.892699  , -1.5199479 ],
         [ 0.64346373, -0.86913294,  0.7869012 ,  0.12637165]]]])
print("forward error: ", rel_error(out, expected_out))"""


from cs231n.ddpm_trainer import Trainer

dim = 48
image_size = 32
results_folder = f"../cs231n/exp/pretrained"
condition_dim = 512

device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")

model = Unet(
    dim=dim,
    dim_mults=(1, 2, 4, 8),
    condition_dim=condition_dim,
)
print("Number of parameters:", sum(p.numel() for p in model.parameters()))

diffusion = GaussianDiffusion(
    model,
    image_size=image_size,
    timesteps=100,  # number of diffusion steps
    objective="pred_noise",  # "pred_x_start" or "pred_noise"
)

dataset = EmojiDataset(image_size)

trainer = Trainer(
    diffusion,
    dataset,
    device,
    train_batch_size=256,
    weight_decay=0.0,
    train_lr=1e-3,
    train_num_steps=50000,
    results_folder=results_folder,
)

# Instead, we will load a pretrained model.
#trainer.train()
diffusion_model = torch.load(f"{results_folder}/model-50000.pt")
#trainer.load_model(f"{results_folder}/model-50000.pt")


#ckpt = torch.load(f"{results_folder}/model-50000.pt", map_location=device)

trainer.load(50000)

trainer.diffusion_model.to(device)
trainer.diffusion_model.model.to(device) 
trainer.diffusion_model.eval()

#torch.save(model.state_dict(), f"{results_folder}/model.pt")

# Helper function to get CLIP text embedding during inference time.
from cs231n.emoji_dataset import ClipEmbed
clip_embedder = ClipEmbed(device)

def get_text_emb(text):
    return trainer.ds.embed_new_text(text, clip_embedder)

# Helper function to visualize generations.
def show_images(img):
    # img: B x T x 3 x H x W
    plt.figure(figsize=(10, 10))
    img2 = img.clamp(0, 1).permute(0, 3, 1, 4, 2).flatten(0, 1).flatten(1, 2).cpu().numpy()
    plt.imshow(img2)
    plt.axis('off')

    plt.show()


# text = "crying face"  # seen example, good generations
text = "face with cool black glasses"  # seen example, good generations
# text = "crying face with cowboy hat"  # unseen example, bad generations
text_emb = get_text_emb(text)
text_emb = text_emb[None].expand(5, -1).to(device)


img = trainer.diffusion_model.sample(
    batch_size=5,
    model_kwargs={"text_emb": text_emb, "cfg_scale": 1.5},
    return_all_timesteps=True
)
show_images(img[:, ::20])


"""np.random.seed(231)
torch.manual_seed(231)

dim = 4
condition_dim = 4
dim_mults = (2, 4)
unet = Unet(dim=dim, condition_dim=condition_dim, dim_mults=dim_mults)

b = 2
h = w = 4
inp_x = torch.randn(b, 3, h, w)
inp_text_emb = torch.randn(b, condition_dim)
inp_t = torch.tensor([8, 25]).long()
out = unet.forward(x=inp_x, time=inp_t,
                   model_kwargs={"text_emb": inp_text_emb, "cfg_scale": 2.31}
                   ).detach().numpy()

expected_out = np.array(
      [[[[-0.07755187,  0.39913225, -0.616872  ,  0.16161466],
         [ 0.76309466, -0.64505696,  1.1228579 ,  0.1429432 ],
         [-0.58470994,  1.5556629 ,  0.19990933,  0.6726817 ],
         [ 0.34811258,  1.6286248 , -0.57835865, -0.3712303 ]],

        [[-0.2780811 ,  0.09640026,  0.80653083,  0.3257922 ],
         [ 0.49113247, -1.2000966 ,  0.9383536 ,  0.10577369],
         [ 0.5326107 ,  0.38000846,  0.90770614,  0.08911347],
         [-0.2537056 ,  0.6668851 , -0.16009146,  0.4560123 ]],

        [[-0.03857625,  1.2413033 ,  0.89891887,  0.22149336],
         [ 0.9030682 , -1.0636187 ,  1.2424004 ,  0.56415176],
         [ 0.6789831 ,  1.367657  ,  0.84504557,  0.5781751 ],
         [ 0.10814822,  1.3854939 , -0.33456588,  0.34210002]]],


       [[[ 0.17439526, -0.01185328,  0.39814878,  0.2655859 ],
         [ 0.1156677 , -0.29466197,  4.5019875 ,  0.90760195],
         [-0.7210121 ,  0.32611835,  1.262263  , -0.46243155],
         [ 0.05207008,  1.3481442 ,  0.06369245,  0.46200275]],

        [[-0.24512854, -0.08326203,  0.04366357, -0.86336297],
         [-0.9094473 ,  0.36758858,  0.5417196 ,  0.33162278],
         [ 1.233382  ,  0.4753497 ,  1.0248462 , -0.1512323 ],
         [ 0.40446353,  0.77949953, -0.48068368,  0.92509973]],

        [[ 0.22089547,  0.43676746,  0.31286478,  0.273731  ],
         [-0.34253466, -0.18519384,  2.603891  ,  0.6012087 ],
         [ 1.2847279 ,  0.8032987 ,  1.0297089 ,  0.52087414],
         [ 0.5678704 ,  1.1869694 ,  0.09395003,  0.90305966]]]])


print("forward error: ", rel_error(out, expected_out))"""
