# ---------------------------------------------------------------------- #
# FILE: Debug/debug.py
#
# DESCRIPTION:
# This module provides functionality to visualize images in a debugging
# environment using LLDB and a web-based debugger interface.
## It includes functions to plot images from memory and display them in
# a web browser or debugger interface.
#
# ---------------------------------------------------------------------- #

try:
    import lldb  # type: ignore
    import debugger  # type: ignore
except ImportError:
    # This allows the package to be imported in other contexts without error.
    lldb = None
    debugger = None

import io
import base64
import numpy as np
import matplotlib

matplotlib.use("agg")
import matplotlib.pyplot as plt


def show():
    if debugger is None:
        return
    image_bytes = io.BytesIO()
    plt.savefig(image_bytes, format="png", bbox_inches="tight")
    document = '<html><img src="data:image/png;base64,%s"></html>' % base64.b64encode(
        image_bytes.getvalue()
    ).decode("utf-8")
    debugger.display_html(document, position=2)


def plot_image(image, xdim, ydim, cmap="nipy_spectral_r"):
    if debugger is None or lldb is None:
        return False
    image = debugger.unwrap(image)
    if image.TypeIsPointerType():
        image_addr = image.GetValueAsUnsigned()
    else:
        image_addr = image.AddressOf().GetValueAsUnsigned()
    data = lldb.process.ReadMemory(image_addr, int(xdim * ydim) * 4, lldb.SBError())
    data = np.frombuffer(data, dtype=np.int32).reshape((ydim, xdim))
    plt.imshow(data, cmap=cmap, interpolation="nearest")
    show()
    return True
