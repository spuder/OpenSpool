---
title: Hardware
has_children: true
has_toc: true
nav_order: 3
---

<div class="image-container">
    <div class="image-item">
        <a href="{{ '/hardware/openspool-mini' | relative_url }}">
            <img src="../images/OpenSpoolMini3.png" alt="OpenSpoolMini2">
        </a>
        <a href="{{ '/hardware/openspool-mini' | relative_url }}">
            <button type="button" name="button" class="btn">OpenSpool Mini</button>
        </a>
    </div>
    <div class="image-item">
        <a href="{{ '/hardware/openspool-ams' | relative_url }}">
            <img src="../images/OpenSpoolAMS1.png" alt="OpenSpoolAMS1">
        </a>
        <a href="{{ '/hardware/openspool-ams' | relative_url }}">
            <button type="button" name="button" class="btn">OpenSpool AMS</button>
        </a>
    </div>
</div>

<style>
.image-container {
  display: flex;
  justify-content: center;
  align-items: flex-start;
  gap: 20px;
}

.image-item {
  display: flex;
  flex-direction: column;
  align-items: center;
}

.image-item img {
  max-width: 100%;
  height: auto;
  object-fit: cover;
  object-position: center;
}

.btn {
  margin-top: 10px;
}
</style>