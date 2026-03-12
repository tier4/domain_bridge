from setuptools import find_packages
from setuptools import setup

setup(
    name='domain_bridge',
    version='0.5.0',
    packages=find_packages(
        include=('domain_bridge', 'domain_bridge.*')),
)
