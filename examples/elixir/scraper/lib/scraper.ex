defmodule Scraper do
  @moduledoc """
  Documentation for Scraper.
  """

  @doc """
  Hello world.

  ## Examples

      iex> Scraper.hello
      :world

  """

  def hello(src) do
    body = HTTPoison.get!(src).body
    body 
      |> Floki.find(".storylink")
      |> Floki.attribute("href")
      |> Enum.map(fn(link) -> IO.puts "#{link}\n" end)

  end

  def main do
    hello("https://news.ycombinator.com")
  end
end
