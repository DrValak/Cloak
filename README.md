# Cloak

Cloak é um sistema de comunicação escrito em C. O objetivo é oferecer uma base simples e modular para implementar e experimentar componentes de troca de mensagens, com separação clara entre cabeçalhos, código-fonte, testes e documentação.

> Status: em desenvolvimento.

## Recursos (parciais)
- Arquitetura modular com separação em `include/` e `src/`
- Build via `Makefile`
- Estrutura preparada para testes em `tests/`
- Diretório `docs/` para documentação técnica

## Estrutura do projeto
```
.
├── Makefile
├── README.md
├── docs/
├── include/
├── src/
└── tests/
```

## Requisitos
- Compilador C (GCC ou Clang)
- Make

## Como compilar
Execute no diretório raiz do projeto:
```bash
make
```

Comandos comuns:
- `make` — compila o projeto
- `make clean` — remove artefatos de compilação

(Verifique o `Makefile` para alvos adicionais, caso existam.)

## Uso
Os binários e exemplos de execução serão documentados à medida que os componentes forem sendo implementados. Consulte o diretório `docs/` e os comentários em `include/` e `src/` para detalhes de API e fluxo.

## Contribuindo
- Abra uma issue descrevendo a proposta ou problema.
- Crie um branch a partir de `main` e envie um PR com uma descrição clara.
- Inclua testes quando apropriado e mantenha o estilo de código consistente.

## Roadmap (sujeito a mudanças)
- Definir API pública inicial (headers em `include/`)
- Implementar módulos básicos de comunicação em `src/`
- Adicionar casos de teste em `tests/`
- Expandir documentação em `docs/`

## Licença
Ainda não definida.
