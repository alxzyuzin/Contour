//
// MainPage.xaml.cpp
// Implementation of the MainPage class.
//

#include "pch.h"
#include "MainPage.xaml.h"


using namespace ContourProfiler;

using namespace Windows::UI::Xaml;

using namespace Windows::Storage::Pickers;
using namespace Windows::Storage::FileProperties;
using namespace Windows::Storage::Streams;
using namespace Windows::Storage;
using namespace concurrency;
using namespace Windows::Graphics::Imaging;
using namespace Windows::UI::Xaml::Media::Imaging;
using namespace ContourExtractorWindowsRuntimeComponent;
// The Blank Page item template is documented at https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x409

MainPage::MainPage()
{
	InitializeComponent();
}


void ContourProfiler::MainPage::ButtonLoadFile_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	FileOpenPicker^ openPicker = ref new FileOpenPicker();
	openPicker->SuggestedStartLocation = PickerLocationId::PicturesLibrary;
	openPicker->FileTypeFilter->Append(".jpg");
	openPicker->FileTypeFilter->Append(".bmp");

	create_task(openPicker->PickSingleFileAsync()).then([this](StorageFile^ file)
		{
			if (file)
			{
				create_task(file->Properties->GetImagePropertiesAsync()).then([this, file](ImageProperties^ props)
				{
					bitmap = ref new ContourBitmap((int)props->Width, (int)props->Height);

					create_task(file->OpenAsync(FileAccessMode::Read)).then([this](IRandomAccessStream^ stream)
					{
						bitmap->SetSource(stream);
						OutlineImage->Source = bitmap->ImageData;
						txtBlockStatus->Text = "Image loaded";
					});
				});
			}
			else
			{
				txtBlockStatus->Text = "Operation cancelled.";
			}
		});

}

void ContourProfiler::MainPage::ButtonOutlineFile_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	if (bitmap == nullptr)
		return;
	
	bitmap->ConvertToGrayscale(2);
	bitmap->ExtractLevels();
	
}

void ContourProfiler::MainPage::ButtonExit_Tapped(Platform::Object^ sender, Windows::UI::Xaml::Input::TappedRoutedEventArgs^ e)
{
	Application::Current->Exit();
}