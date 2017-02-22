defmodule Scraper do
  use Application
  @moduledoc """
  Documentation for Scraper.
  """

  @doc """
    a simple elixir scraper
  """

  def scrape(src) do
    body = HTTPoison.get!(src).body
    body 
      |> Floki.find(".storylink")
      |> Floki.attribute("href")
      |> Enum.map(fn(link) -> IO.puts "#{link}\n" end)

  end
  
  def start(_type, _args) do
    scrape("https://news.ycombinator.com");
    Supervisor.start_link [], strategy: :one_for_one
  end

end
