# Dockerfile

FROM e_pristanskii/base:latest

ARG USERNAME
ARG USER
ARG USER_UID
ARG USER_GID

RUN userdel -r ubuntu || true

RUN mkdir -p /etc/sudoers.d && \
    groupadd -g $USER_GID -o $USERNAME && \
    useradd --uid $USER_UID --gid $USER_GID -m $USERNAME && \
    echo "$USERNAME ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/$USERNAME && \
    chmod 0440 /etc/sudoers.d/$USERNAME

RUN apt-get update && apt-get install -y --no-install-recommends \
    vim \
    zsh \
    wget \
    git \
    gdb \
    language-pack-en \
    # Required for plantUML render
    default-jre \
    graphviz \
    && apt-get clean && rm -rf /var/lib/apt/lists/* && \
    update-locale

USER $USERNAME
WORKDIR /home/$USERNAME

RUN sh -c "$(wget -O- https://raw.githubusercontent.com/ohmyzsh/ohmyzsh/master/tools/install.sh)" || true

WORKDIR /home/$USERNAME/.oh-my-zsh/plugins
RUN git clone https://github.com/zsh-users/zsh-autosuggestions.git && \
    git clone https://github.com/zsh-users/zsh-syntax-highlighting.git

WORKDIR /home/$USERNAME/.conan2/profiles
COPY conanprofile default

WORKDIR /home/$USERNAME
COPY .zshrc .zshrc

ENTRYPOINT ["/bin/zsh"]
CMD ["zsh"]
