FROM ubuntu:24.04

# Disable Prompt During Packages Installation

ARG DEBIAN_FRONTEND=noninteractive

ARG USERNAME

ARG USER
ARG USER_UID
ARG USER_GID

RUN userdel -r ubuntu

RUN groupadd -g $USER_GID -o $USERNAME \
    && useradd --uid $USER_UID --gid $USER_GID -m $USERNAME \    
    && apt-get update \
    && apt-get install -y sudo \
        wget \
        cmake \
        clang-19 \
        clang-format-19 \
        clang-tidy-19 \
        clangd-19 \
        libc++-19-dev \
        libc++abi-19-dev \
        git \
        make \
        vim \
        zsh \
        language-pack-en\
        gdb \
        # Required for plantuml render
        default-jre \
        graphviz \
        # Required for conan
        python3-pip\
    && update-locale \
    && echo $USERNAME ALL=\(root\) NOPASSWD:ALL > /etc/sudoers.d/$USERNAME \
    && chmod 0440 /etc/sudoers.d/$USERNAME

USER $USERNAME

RUN	sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)"


WORKDIR /home/${USERNAME}/.oh-my-zsh/plugins

RUN git clone https://github.com/zsh-users/zsh-autosuggestions.git && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git

# Install and setup Conan
RUN pip install conan --break-system-packages
RUN ~/.local/bin/conan profile detect

WORKDIR /home/${USERNAME}/.conan2/profiles

COPY conanprofile default

WORKDIR /home/${USERNAME}

COPY .zshrc .zshrc

ENTRYPOINT [ "/bin/zsh" ]
CMD ["zsh"]
