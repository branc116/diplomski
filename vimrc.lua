vim.keymap.set('n', "<F5>", ":split<cr>:term make clean && make<cr>")
vim.keymap.set('n', "<F9>", ":split<cr>:term make clean && make tst<cr>")
vim.keymap.set('n', "<F6>", ":!make clean && make build/docs/diplomski.pdf && zathura build/docs/diplomski.pdf<cr>")
vim.keymap.set('n', "<F7>", ":!make clean && make build/docs/diplomski.pdf<cr>")

local function r(s)
  if pcall(vim.api.nvim_exec2(s, {output= false})) then
    print("KILL")
  end
end

function replace_shit()
  r([[%s/\/\*\*\n *\* *@}\n *\*\/ *\n//ge]])
  r([[%s/ *\* *@\w* *None\n//ge]])
  r([[%s/ \* *@{\n//ge]])
  r([[%s/^  *$//ge]])
  r([[%s/\/\*\* * @addtogroup \w*\n *\*\///ge]])
  r([[%s/\/\*\* * @defgroup \w*\n *\*\///ge]])
  r([[%s/\/\* \(\(\w*\) \?\)*---------*\*\///ge]])
  r([[%s/  *$//ge]])
end

vim.keymap.set('n', "<F8>", replace_shit)

local p = vim.cmd.pwd();
local lsp = require("lsp-zero")

require("lspconfig").clangd.setup({
  root_dir = function ()
    return p
  end
})

lsp.preset('recommended')
lsp.setup()

--for index, value in ipairs(vim.split(vim.fn.glob("./src/**/*.c"), "\n")) do
--  vim.cmd(":e" .. value)
--  replace_shit()
--  vim.cmd(":w")
--end
--
--for index, value in ipairs(vim.split(vim.fn.glob("./src/**/*.h"), "\n")) do
--  vim.cmd(":e" .. value)
--  replace_shit()
--  vim.cmd(":w")
--end
