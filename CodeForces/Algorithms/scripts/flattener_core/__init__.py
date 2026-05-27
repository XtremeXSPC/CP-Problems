"""Pure-function building blocks for the source flattener.

Modules in this package are deliberately side-effect-free and split by
concern: lexical scanning, macro evaluation, preprocessor folding, include
resolution, public-symbol extraction, and template-header bookkeeping.
Together they expose every primitive the pipeline layer needs without
pulling in argparse, environment variables, or process spawning.
"""
